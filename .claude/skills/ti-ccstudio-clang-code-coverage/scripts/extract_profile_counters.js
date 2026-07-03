// extract_profile_counters.js
//
// Reads LLVM profile counter data and MC/DC bitmap data from target memory
// and writes the concatenated raw bytes to the specified output file.
//
// Usage:
//   run.sh extract_profile_counters.js <ccxml> <executable> <output> [options]
//
// Options:
//   --core <pattern>     Core name regex pattern (default: matches first core)
//   --timeout <ms>       Scripting timeout in milliseconds (default: 60000)
//
// Prerequisites:
//   The program must already be loaded and executed on the target so that the
//   profiling counter and MC/DC bitmap sections contain valid data. This script
//   will load the program (to resolve symbols) and then read the counter data.

const { existsSync, writeFileSync } = require("fs");
const { resolve } = require("path");

function parseArgs() {
	const args = process.argv.slice(3);
	const opts = {
		ccxml: null,
		executable: null,
		output: null,
		corePattern: null,
		timeout: 60000,
	};
	const positional = [];

	for (let i = 0; i < args.length; i++) {
		switch (args[i]) {
			case "--timeout": {
				const val = parseInt(args[++i], 10);
				if (isNaN(val) || val <= 0) {
					console.error("Error: --timeout must be a positive integer");
					process.exit(1);
				}
				opts.timeout = val;
				break;
			}
			case "--core": {
				const pattern = args[++i];
				if (!pattern) {
					console.error("Error: --core requires a pattern");
					process.exit(1);
				}
				opts.corePattern = new RegExp(pattern, "i");
				break;
			}
			default:
				positional.push(args[i]);
				break;
		}
	}

	if (positional.length < 3) {
		console.error(
			"Usage: run.sh extract_profile_counters.js <ccxml> <executable> <output> " +
			"[--core <pattern>] [--timeout <ms>]"
		);
		process.exit(1);
	}

	opts.ccxml      = resolve(positional[0]);
	opts.executable = resolve(positional[1]);
	opts.output     = resolve(positional[2]);

	if (!existsSync(opts.ccxml)) {
		console.error(`Error: CCXML file not found: ${opts.ccxml}`);
		process.exit(1);
	}
	if (!existsSync(opts.executable)) {
		console.error(`Error: Executable file not found: ${opts.executable}`);
		process.exit(1);
	}

	if (!opts.corePattern) {
		opts.corePattern = /.*/;
	}
	return opts;
}

const opts = parseArgs();
const bitSize = 8;

const valuesToBuffer = (values) => Buffer.from(values.map(Number));

function tryEvaluateSymbol(session, symbol) {
	try {
		return session.expressions.evaluate(symbol);
	} catch (e) {
		return null;
	}
}

const ds = initScripting();
ds.setScriptingTimeout(opts.timeout);

try {
	ds.configure(opts.ccxml);
	const session = ds.openSession(opts.corePattern);
	session.target.connect();

	// Load the program so that linker symbols are available for expression evaluation.
	session.symbols.add(opts.executable);

	const cntStart = tryEvaluateSymbol(session, "__start___llvm_prf_cnts");
	const cntStop  = tryEvaluateSymbol(session, "__stop___llvm_prf_cnts");
	if (cntStart === null || cntStop === null) {
		throw new Error("Counter section symbols not found. Was the program compiled with profiling enabled?");
	}

	const cntBytes = Number(cntStop - cntStart);
	console.log(
		`Counter section: 0x${cntStart.toString(16)} – 0x${cntStop.toString(16)} (${cntBytes} bytes)`
	);

	let cntValues, mcdcValues;
	if (cntBytes) cntValues = session.memory.read(cntStart, cntBytes, bitSize);

	const mcdcStart = tryEvaluateSymbol(session, "__start___llvm_prf_bits");
	const mcdcStop  = tryEvaluateSymbol(session, "__stop___llvm_prf_bits");
	const mcdcBytes = (mcdcStart !== null && mcdcStop !== null)
		? Number(mcdcStop - mcdcStart)
		: 0;

	if (mcdcBytes) {
		console.log(
			`MC/DC section:   0x${mcdcStart.toString(16)} – 0x${mcdcStop.toString(16)} (${mcdcBytes} bytes)`
		);
		mcdcValues = session.memory.read(mcdcStart, mcdcBytes, bitSize);
	} else {
		console.log("MC/DC section:   not present or empty");
	}

	const output = Buffer.concat([
		valuesToBuffer(cntValues || []),
		valuesToBuffer(mcdcValues || [])
	]);
	const outputPath = opts.output;

	writeFileSync(outputPath, output);
	console.log(`Wrote ${output.length} bytes to ${outputPath}`);
} catch (err) {
	console.error(err);
	process.exitCode = 1;
} finally {
	ds.shutdown();
}
