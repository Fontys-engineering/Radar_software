<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.8.20" doxygen_gitid="f246dd2f1c58eea39ea3f50c108019e4d4137bd5">
  <compound kind="file">
    <name>cli.h</name>
    <path>/nightlybuilds/mmwave_app_sdk/ti/utils/cli/</path>
    <filename>cli_8h.html</filename>
    <class kind="struct">CLI_CmdTableEntry_t</class>
    <class kind="struct">CLI_Cfg_t</class>
    <member kind="define">
      <type>#define</type>
      <name>CLI_MAX_CMD</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n.html</anchorfile>
      <anchor>gab31e3fb23ca0f47c369749396a9b6ce6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLI_MAX_ARGS</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n.html</anchorfile>
      <anchor>ga3c501e1e7423fe21c13d71fdf5740590</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void *</type>
      <name>CLI_Handle</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>gaed79daab87256a2978ab7cf40bc15393</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>CLI_CmdHandler</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>ga28d76e5d5a29c3e5335381264451d58e</anchor>
      <arglist>)(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="typedef">
      <type>struct CLI_CmdTableEntry_t</type>
      <name>CLI_CmdTableEntry</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>ga80b24e8a698a1c807235abc76d0a27d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct CLI_Cfg_t</type>
      <name>CLI_Cfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>ga25790460fdd028e95fbebd5b7af57515</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_open</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga664861a70519c2c7c5e4091b1a0958f2</anchor>
      <arglist>(CLI_Cfg *ptrCLICfg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CLI_write</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga115d0ce3ad0a40b0d11746e38947a110</anchor>
      <arglist>(const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_close</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafd45fb25e5c1cc3f434b4de1270391d4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CLI_getMMWaveExtensionConfig</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2a2dd5f51156608cdc22ed334f828e41</anchor>
      <arglist>(MMWave_CtrlCfg *ptrCtrlCfg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CLI_getMMWaveExtensionOpenConfig</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga6115df47a85eb4ef36f5ff0c88369c1a</anchor>
      <arglist>(MMWave_OpenCfg *ptrOpenCfg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>cli_internal.h</name>
    <path>/nightlybuilds/mmwave_app_sdk/ti/utils/cli/include/</path>
    <filename>cli__internal_8h.html</filename>
    <includes id="cli_8h" name="cli.h" local="no" imported="no">ti/utils/cli/cli.h</includes>
    <class kind="struct">CLI_MCB_t</class>
    <class kind="struct">CLI_partInfoString_t</class>
    <member kind="typedef">
      <type>struct CLI_MCB_t</type>
      <name>CLI_MCB</name>
      <anchorfile>cli__internal_8h.html</anchorfile>
      <anchor>a0a4ff9d4aa7ac3c7a1ad8a5009e69418</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct CLI_partInfoString_t</type>
      <name>CLI_partInfoString</name>
      <anchorfile>cli__internal_8h.html</anchorfile>
      <anchor>a658bda5bdbdf12a4d7df1f3bed41f729</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_MMWaveExtensionInit</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab7d2f34af7eb456da1eebe523d7e8d32</anchor>
      <arglist>(CLI_Cfg *ptrCLICfg)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_MMWaveExtensionHandler</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2ec9b0beaba066deafc63850d465bd8b</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CLI_MMWaveExtensionHelp</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafeccc3d29da2612038145bfb3da68f10</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>CLI_MCB</type>
      <name>gCLI</name>
      <anchorfile>cli__internal_8h.html</anchorfile>
      <anchor>aff95d31319af774a7c30437c34580f86</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CLI_Cfg_t</name>
    <filename>struct_c_l_i___cfg__t.html</filename>
    <member kind="variable">
      <type>char *</type>
      <name>cliPrompt</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>a466d8dc43476a17a29507055fd3ba692</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cliBanner</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>ab99630421352e9195c75eb1df738d004</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UART_Handle</type>
      <name>cliUartHandle</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>adbb13374d6d9b65cd8b54ab675249c46</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>enableMMWaveExtension</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>afceff556435cee60ab16107d52b31835</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MMWave_Handle</type>
      <name>mmWaveHandle</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>ae853d16c3176659f55576228c3108acf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>taskPriority</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>ad541c61e9322c58a59773331950fd3c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>usePolledMode</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>acab57d5decb782548d1a2b72947b4444</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>overridePlatform</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>adafd2584e72165ab09b18621bdef7c35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>procChain</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>ad5572a81877657a053e65ba1ae05f9a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>overridePlatformString</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>ad82a6a2b62f21e8b960a18ae94708114</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CLI_CmdTableEntry</type>
      <name>tableEntry</name>
      <anchorfile>struct_c_l_i___cfg__t.html</anchorfile>
      <anchor>a26775782e797b6c70806b5d4927eff35</anchor>
      <arglist>[CLI_MAX_CMD]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CLI_CmdTableEntry_t</name>
    <filename>struct_c_l_i___cmd_table_entry__t.html</filename>
    <member kind="variable">
      <type>char *</type>
      <name>cmd</name>
      <anchorfile>struct_c_l_i___cmd_table_entry__t.html</anchorfile>
      <anchor>aaa3d2d7597b567734aa077208745a18d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>helpString</name>
      <anchorfile>struct_c_l_i___cmd_table_entry__t.html</anchorfile>
      <anchor>affbc4a7a9c4d5dd8d6300fbe394c2e5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CLI_CmdHandler</type>
      <name>cmdHandlerFxn</name>
      <anchorfile>struct_c_l_i___cmd_table_entry__t.html</anchorfile>
      <anchor>a03220ae97cb3885b21377821f3a754df</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CLI_MCB_t</name>
    <filename>struct_c_l_i___m_c_b__t.html</filename>
    <member kind="variable">
      <type>CLI_Cfg</type>
      <name>cfg</name>
      <anchorfile>struct_c_l_i___m_c_b__t.html</anchorfile>
      <anchor>afda1bbadece68c530bd35b6a2181a64d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>numCLICommands</name>
      <anchorfile>struct_c_l_i___m_c_b__t.html</anchorfile>
      <anchor>a145224d65efcb9bb3dabf72297df6073</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TaskHandle_t</type>
      <name>cliTaskHandle</name>
      <anchorfile>struct_c_l_i___m_c_b__t.html</anchorfile>
      <anchor>aa6eb5d5a9b7f5db04aa2c00e5ac891df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SemaphoreP_Object</type>
      <name>cliBypasssemaphoreObj</name>
      <anchorfile>struct_c_l_i___m_c_b__t.html</anchorfile>
      <anchor>a34f9ea40655c03b1eb2836569d9a6032</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CLI_partInfoString_t</name>
    <filename>struct_c_l_i__part_info_string__t.html</filename>
  </compound>
  <compound kind="group">
    <name>CLI_UTIL</name>
    <title>CLI Utility</title>
    <filename>group___c_l_i___u_t_i_l.html</filename>
    <subgroup>CLI_UTIL_EXTERNAL_FUNCTION</subgroup>
    <subgroup>CLI_UTIL_EXTERNAL_DATA_STRUCTURE</subgroup>
    <subgroup>CLI_UTIL_EXTERNAL_DEFINITION</subgroup>
    <subgroup>CLI_UTIL_INTERNAL_FUNCTION</subgroup>
    <subgroup>CLI_UTIL_INTERNAL_DATA_STRUCTURE</subgroup>
  </compound>
  <compound kind="group">
    <name>CLI_UTIL_EXTERNAL_FUNCTION</name>
    <title>CLI Utility External Functions</title>
    <filename>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</filename>
    <member kind="function">
      <type>void</type>
      <name>CLI_write</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga115d0ce3ad0a40b0d11746e38947a110</anchor>
      <arglist>(const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_open</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga664861a70519c2c7c5e4091b1a0958f2</anchor>
      <arglist>(CLI_Cfg *ptrCLICfg)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_close</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafd45fb25e5c1cc3f434b4de1270391d4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CLI_getMMWaveExtensionConfig</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2a2dd5f51156608cdc22ed334f828e41</anchor>
      <arglist>(MMWave_CtrlCfg *ptrCtrlCfg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CLI_getMMWaveExtensionOpenConfig</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga6115df47a85eb4ef36f5ff0c88369c1a</anchor>
      <arglist>(MMWave_OpenCfg *ptrOpenCfg)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>CLI_UTIL_EXTERNAL_DATA_STRUCTURE</name>
    <title>CLI Utility External Data Structures</title>
    <filename>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</filename>
    <class kind="struct">CLI_CmdTableEntry_t</class>
    <class kind="struct">CLI_Cfg_t</class>
    <member kind="typedef">
      <type>void *</type>
      <name>CLI_Handle</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>gaed79daab87256a2978ab7cf40bc15393</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>CLI_CmdHandler</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>ga28d76e5d5a29c3e5335381264451d58e</anchor>
      <arglist>)(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="typedef">
      <type>struct CLI_CmdTableEntry_t</type>
      <name>CLI_CmdTableEntry</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>ga80b24e8a698a1c807235abc76d0a27d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct CLI_Cfg_t</type>
      <name>CLI_Cfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</anchorfile>
      <anchor>ga25790460fdd028e95fbebd5b7af57515</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>CLI_UTIL_EXTERNAL_DEFINITION</name>
    <title>CLI Utility External Definitions</title>
    <filename>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>CLI_MAX_CMD</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n.html</anchorfile>
      <anchor>gab31e3fb23ca0f47c369749396a9b6ce6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLI_MAX_ARGS</name>
      <anchorfile>group___c_l_i___u_t_i_l___e_x_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n.html</anchorfile>
      <anchor>ga3c501e1e7423fe21c13d71fdf5740590</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>CLI_UTIL_INTERNAL_FUNCTION</name>
    <title>CLI Utility Internal Functions</title>
    <filename>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</filename>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_help</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gae92812e5620dd984a3ff0a02bcf2969e</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>CLI_task</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga31f4df472e565b452bf04974ac1afe00</anchor>
      <arglist>(void *args)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveVersion</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gacc2a57cf2cf11a6b97b2e8f338ca2e6d</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveFlushCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga42c989058962c57a8a712cd8c66d37a9</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveDataOutputMode</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaf2b5176018ae1eef1423c2bdd8d1718a</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveChannelCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gad3e36eede4e75cfb2e958376f1dfc62c</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveADCCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga91fe49d3fa81a919ee3c340e89b0313d</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveProfileCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gad1f5fe8556a5e33019b3ee30ea5166bd</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveChirpCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaeeee3d22f9e33c6a16511a5a76d475e1</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveAdvChirpCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gad5674bdd4ed09a951dc44379315175b3</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveLUTDataCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga4bbafc107a37f13698a4cb5cda8c86dc</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveFrameCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaa062d01aa39f41deb774fee52e255f58</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveAdvFrameCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga7225f239addecbf5976d820c9377096d</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveSubFrameCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2bca55e9da14c5040fac05207d5549e9</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveAdvFrameDataCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga89f70c0eedb9c60abcd2fb1a0da44094</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveLowPowerCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga3b16b959d00715c834f434c6127465b2</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveContModeCfg</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga5041101674f71000712644974e6c4919</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveSetLdoBypassConfig</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gac534daf91a7bb212f1b22f82d8562e8a</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveApllSynthBwConfig</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gac0c252d8883fa5ad6806d58d40ae4905</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>CLI_MMWaveBPMCfgAdvanced</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gae92bed483db16977efa0273f404ce765</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_MMWaveExtensionInit</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab7d2f34af7eb456da1eebe523d7e8d32</anchor>
      <arglist>(CLI_Cfg *ptrCLICfg)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>CLI_MMWaveExtensionHandler</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2ec9b0beaba066deafc63850d465bd8b</anchor>
      <arglist>(int32_t argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CLI_MMWaveExtensionHelp</name>
      <anchorfile>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafeccc3d29da2612038145bfb3da68f10</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>CLI_UTIL_INTERNAL_DATA_STRUCTURE</name>
    <title>CLI Utility Internal Data Structures</title>
    <filename>group___c_l_i___u_t_i_l___i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e.html</filename>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>CLI Utility</title>
    <filename>index.html</filename>
  </compound>
</tagfile>
