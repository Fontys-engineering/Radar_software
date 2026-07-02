<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.8.20" doxygen_gitid="f246dd2f1c58eea39ea3f50c108019e4d4137bd5">
  <compound kind="file">
    <name>objectdetectioninternal.h</name>
    <path>/nightlybuilds/mmwave_app_sdk/ti/datapath/dpc/objectdetection/objdethwa/include/</path>
    <filename>objectdetectioninternal_8h.html</filename>
    <includes id="objectdetection_8h" name="objectdetection.h" local="no" imported="no">ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h</includes>
    <class kind="struct">DpuConfigs_t</class>
    <class kind="struct">SubFrameObj_t</class>
    <class kind="struct">MemPoolObj_t</class>
    <class kind="struct">Element_t</class>
    <class kind="struct">ObjDetObj_t</class>
    <member kind="define">
      <type>#define</type>
      <name>DPC_Objdet_Assert</name>
      <anchorfile>objectdetectioninternal_8h.html</anchorfile>
      <anchor>a04285bb51af8760098791f8a2b9cae9d</anchor>
      <arglist>(handle, expression)</arglist>
    </member>
    <member kind="typedef">
      <type>struct DpuConfigs_t</type>
      <name>DpuConfigs</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gacba76ea4873414351f89cbae8b152002</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct SubFrameObj_t</type>
      <name>SubFrameObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga66223d3fe88a478232a4baa67bdb5920</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct MemPoolObj_t</type>
      <name>MemPoolObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga788ff8f9775b0058d4fbaea936649d68</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct Element_t</type>
      <name>Element</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga57b5a60faaa35f7409b5d1b9828e7adc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct ObjDetObj_t</type>
      <name>ObjDetObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga840cbb1d253d01785da21bc735411d6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_DPC_Objdet_Assert</name>
      <anchorfile>objectdetectioninternal_8h.html</anchorfile>
      <anchor>ab829cad7a9d82c2e96650a61af1605cf</anchor>
      <arglist>(DPM_Handle handle, int32_t expression, const char *file, int32_t line)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>objectdetection.h</name>
    <path>/nightlybuilds/mmwave_app_sdk/ti/datapath/dpc/objectdetection/objdethwa/</path>
    <filename>objectdetection_8h.html</filename>
    <class kind="struct">DPC_ObjectDetection_CalibDcRangeSigCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_CfarCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_fovRangeCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_fovDopplerCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_MultiObjBeamFormingCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_fovAoaCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_extMaxVelCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_RangeAzimuthHeatMapCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_StaticClutterRemovalCfg_Base_t</class>
    <class kind="struct">DPC_ObjectDetection_StaticClutterRemovalCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_MeasureRxChannelBiasCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_ProcessCallBackCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_MemCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_InitParams_t</class>
    <class kind="struct">DPC_ObjectDetection_StaticCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_DynCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_AntGeometryCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_PreStartCommonCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage_t</class>
    <class kind="struct">DPC_ObjectDetection_PreStartCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_Stats_t</class>
    <class kind="struct">DPC_ObjectDetection_ExecuteResult_t</class>
    <class kind="struct">DPC_ObjectDetection_ExecuteResultExportedInfo_t</class>
    <member kind="define">
      <type>#define</type>
      <name>EDMA_NUM_CC</name>
      <anchorfile>objectdetection_8h.html</anchorfile>
      <anchor>aac95304835bdf821572d6293cd53322d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_MAX_NUM_OBJECTS</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gacc21bf75d7659447df554afa64ae0bc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUM_AZIM_VIRT_ANT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga7d33188163ed5a9de2d22fca4771289c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUM_ELEV_VIRT_ANT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga14a30791bb4128a5cd1e47d983b7f7f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUM_VIRT_ANT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga9cc5752d3b3201b098ff53ee8e6eb772</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga80aa6076fb941f1be8943b04e7e91446</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gadd76b7822011f2beb6aa4ee56e34f1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_CFAR_RANGE_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaec9f74f65466642cc9bfaff59ec80e62</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_CFAR_DOPPLER_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga30e55ff74fd577e375da3f82332d373d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_MULTI_OBJ_BEAM_FORM_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaf6d697c2f90323ec9fad31e8a554d997</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_CALIB_DC_RANGE_SIG_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga2fa5ab168b3acb96f5bbe954330598e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_STATICCLUTTER_REMOVAL_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaaf6c32ded08c76e04a30518b72892952</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga2c431f3f05cc138597dfdfc2ce33e49a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga41ce76942b5dee5914dff7e84a7f0a19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_FOV_RANGE</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaf5ebb36e5c52b6286fef0723a4b0a74f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_FOV_DOPPLER</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga4129c2bab5b30119da5d7491d94ce9b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_FOV_AOA</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga253239459a22f88595b927659a9fc6f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_RANGE_AZIMUTH_HEAT_MAP</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gadf9c7fa80f67d74bd1d7fe4024698a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_EXT_MAX_VELOCITY</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gab22059e0bec77c31a6806be1568127a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gad16a4accd6829309809e215b7861ed4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__TRIGGER_FRAME</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga748dcbeed295626c339215deb7dfe627</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga44b9e9ec1ff5cb005f2ef4a12e28ae73</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL__INIT_CFG_ARGSIZE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga6db2e9f59bdeeda09b4573885b267ea0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL__MEASURE_RX_CHANNEL_BIAS_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga29f7b672f9b0b0446108baf94957db87</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL__COMMAND</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga96cac2f23d951ac189e31070f9959295</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL_ANT_PATTERN</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gac3f1655b8ba3c74612b18899eb1ffcad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga03456a240ccbe4b83bb6439d563acf48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__L3_RAM_RADAR_CUBE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gaf15f666e2ba42dc9b57f18886394ec96</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__L3_RAM_DET_MATRIX</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga21fbd3e8a1a008b2496a696929a5b38b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM_HWA_WINDOW_RAM</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gade825cdfa1c1ba9d3f739a7f0eeabd86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga16c37bf87fb62f456b36579664fb853e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_HWA_WINDOW</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gac7cb577287bc37fe3cb8dad12c97c003</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_CFAR_DOPPLER_DET_OUT_BIT_MASK</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga506348f89e2d32c08c1bac9f73afdb86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_STATIC_CLUTTER_SCRATCH</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gae2026854a04d4d0a5aa08bae373c3342</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_CFAR_OUT_DET_LIST</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga2c7e5b82f8cd90d0c88ff1d57ab8b1fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_OUT</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gacc1146cbaca54f6bf102fb75cd11a3f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_OUT_SIDE_INFO</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gab9491e75ae1b91183c4e1769df76df5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_2_AZIM_IDX</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gaf3ccf4afd5e1aa33ef88fcc84d023d5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_ELEVATION_ANGLE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gafaa15fc6b1c423a92a9f305b7a2582d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_AZIMUTH_STATIC_HEAT_MAP</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga39c0725e76ff7fce1aeec88eb76c82aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_AZIMUTH_FFT_MAGNITUDE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gadde2b79670500cfaba5f0c0e2e7da8c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_SCRATCH_BUFFER</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gabb08d6c6163079e00dfd5bc69f226c90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_PRE_START_CONFIG_BEFORE_PRE_START_COMMON_CONFIG</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga3fd5cf815de0648d09c5424bc00ed6ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINTERNAL</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga5ce1e64d19b80928f043d6436c01078e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOTIMPL</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga6287c2038a0b61d0771d16a3072cdb5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_CalibDcRangeSigCfg_t</type>
      <name>DPC_ObjectDetection_CalibDcRangeSigCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga8da4ca6327faacf817ac462fb5e45b71</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_CfarCfg_t</type>
      <name>DPC_ObjectDetection_CfarCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga653015b26a136a110c07d1b850dce32a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_fovRangeCfg_t</type>
      <name>DPC_ObjectDetection_fovRangeCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gac766a299ef497d723cdbaf5124a253ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_fovDopplerCfg_t</type>
      <name>DPC_ObjectDetection_fovDopplerCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga20922195965064877ab2e557b7cb5f99</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_MultiObjBeamFormingCfg_t</type>
      <name>DPC_ObjectDetection_MultiObjBeamFormingCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gad6d7fa935fbf961705171572d129dcbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_fovAoaCfg_t</type>
      <name>DPC_ObjectDetection_fovAoaCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gabf3e11f4cf8f677ad7dcd5cc8994af63</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_extMaxVelCfg_t</type>
      <name>DPC_ObjectDetection_extMaxVelCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gacd0db685da0e1167c27b1edc515dcd61</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_RangeAzimuthHeatMapCfg_t</type>
      <name>DPC_ObjectDetection_RangeAzimuthHeatMapCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga6f58b7e08d774e961c6feef3a68cea92</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_StaticClutterRemovalCfg_Base_t</type>
      <name>DPC_ObjectDetection_StaticClutterRemovalCfg_Base</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gadff964d923ee4adcdbeada10546789d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_StaticClutterRemovalCfg_t</type>
      <name>DPC_ObjectDetection_StaticClutterRemovalCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gaae0ac9d7b5d6a5043f0db5f88179f6b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_MeasureRxChannelBiasCfg_t</type>
      <name>DPC_ObjectDetection_MeasureRxChannelBiasCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga74947625bac7079aabc9008a3f32428f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>DPC_ObjectDetection_processCallBackFxn_t</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gab0ee2272c61cf46d095887deb89fac29</anchor>
      <arglist>)(uint8_t subFrameIndx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_ProcessCallBackCfg_t</type>
      <name>DPC_ObjectDetection_ProcessCallBackCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gac692d02ace5b44eda283ac29d4d40682</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_MemCfg_t</type>
      <name>DPC_ObjectDetection_MemCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gad7eb47cceeb01abde75cfa49af110423</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_InitParams_t</type>
      <name>DPC_ObjectDetection_InitParams</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gaeb2ab2c35a97b750b34471f7ca9da638</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_StaticCfg_t</type>
      <name>DPC_ObjectDetection_StaticCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga204815ebe03bd22ddde993e03334cf7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_DynCfg_t</type>
      <name>DPC_ObjectDetection_DynCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga1d7e14014463d4d2074956cf8204255e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_AntGeometryCfg_t</type>
      <name>DPC_ObjectDetection_AntGeometryCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga7b27bdfa37c0f7e6089220e20bb587c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_PreStartCommonCfg_t</type>
      <name>DPC_ObjectDetection_PreStartCommonCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga525ff01906976598321804be6ba28e9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage_t</type>
      <name>DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gadf33cf6e1cd3a839847343333bfd9861</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_PreStartCfg_t</type>
      <name>DPC_ObjectDetection_PreStartCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gad708887a74b45cd55835431170ab82cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_Stats_t</type>
      <name>DPC_ObjectDetection_Stats</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga1c01f9da6e20ecb450747a8bdd1bd0bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_ExecuteResult_t</type>
      <name>DPC_ObjectDetection_ExecuteResult</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gaf6613c3cafe1d95c280c1d603a4d4a52</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_ExecuteResultExportedInfo_t</type>
      <name>DPC_ObjectDetection_ExecuteResultExportedInfo</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gac86cb9157a5e88b81f5b22410988003d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPM_ProcChainCfg</type>
      <name>gDPC_ObjectDetectionCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____g_l_o_b_a_l.html</anchorfile>
      <anchor>ga281e7283fc81524fbf3f01d662b8ce8e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>objectdetection.c</name>
    <path>/nightlybuilds/mmwave_app_sdk/ti/datapath/dpc/objectdetection/objdethwa/src/</path>
    <filename>objectdetection_8c.html</filename>
    <includes id="objectdetectioninternal_8h" name="objectdetectioninternal.h" local="no" imported="no">ti/datapath/dpc/objectdetection/objdethwa/include/objectdetectioninternal.h</includes>
    <includes id="objectdetection_8h" name="objectdetection.h" local="no" imported="no">ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>DBG_DPC_OBJDET</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>ade9978ecda4a91523e947c7f62200584</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_HWA_MEM_BANK_INDX_CFARDETMAT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gad3e7dbba6defd511fbdc2327460bbf76</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_HWA_MEM_BANK_INDX_DOPPLEROUT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga810bba96d82ffaa5c7eec631f58c3796</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_HWA_MEM_BANK_INDX_RANGEOUT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gad3d494006d06a49d23f841c72b386782</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gaa8731c03d10d2315e5eadb7f2afd53ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_DET_MATRIX_DATABUF_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga638d914fc7dfb01a653d8d4bbf805251</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gac59d123e24422d9bbfeede1667d53d4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gaeb63c1b2a34f840577a8aebf7b1008ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_CFAR_DET_LIST_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga501d3496615416c113324d0630e752e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gac27e9457ac05b7b1cca50df75707c8a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gab485b4470921a17dcd3cba2af9a65710</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga4cc0b43c12df53c1516a7d6fe59647ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga4950fd5a472db2110b7a35b5bcd047c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gaae738fb0a93d1f1b3a4c1f8826a853bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga0d160f044d6b27d08190840b2cd01860</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gad29d677b3c66671d1653f2e721795013</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga23750a6aa6cb1454bf77d745edb2f4be</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_HWA_MAX_WINDOW_RAM_SIZE_IN_SAMPLES</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a777e36e72114a4a930d2fdeab9f169e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_HWA_NUM_PARAM_SETS</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>ae618600ca97cccc5bb69c52332534592</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_USE_SYMMETRIC_WINDOW_RANGE_DPU</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a809488f39ada9c01e4a375880c970fde</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_USE_SYMMETRIC_WINDOW_DOPPLER_DPU</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a6803aecc2c3e2174dd7a62211348310b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_DPU_RANGEPROC_FFT_WINDOW_TYPE</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>aae77a6eaaadab78dce9bf27b828828f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_DPU_DOPPLERPROC_FFT_WINDOW_TYPE</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a39e9dbd4e6902270a0cf4f4bc2927890</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OBJECT_DETECTION_HEAP_SIZE</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>abcfe7ad8d8f16cee23c776def13e7ccf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OBJECTDETECTION_HEAP_MEM_SIZE</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a5844efdb51ed3d90a8e3755c3ecafe0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_QFORMAT_RANGE_FFT</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>afe6e82123974674e97724f5f71e581ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_QFORMAT_DOPPLER_FFT</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a0fa0166eb5f658d1ae16748a8ccb715e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>compare</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a5ca4cf7d126d1b70e95de936dbf5079d</anchor>
      <arglist>(const void *a, const void *b)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_MemPoolReset</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafa164e3ed6ac64da116723deae8bb2b4</anchor>
      <arglist>(MemPoolObj *pool)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_MemPoolSet</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga82c3045ac8fafd9a6c1ba20438550c7c</anchor>
      <arglist>(MemPoolObj *pool, void *addr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>DPC_ObjDet_MemPoolGet</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga58cd28e5cd493d5e8a8a320469ca45a1</anchor>
      <arglist>(MemPoolObj *pool)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_MemPoolGetMaxUsage</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga058866af95b1947003f708af8a31871e</anchor>
      <arglist>(MemPoolObj *pool)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>DPC_ObjDet_MemPoolAlloc</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2513799b7ea5d07a2d79444e05506ae1</anchor>
      <arglist>(MemPoolObj *pool, uint32_t size, uint8_t align)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DPM_DPCHandle</type>
      <name>DPC_ObjectDetection_init</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab3688b470b396b17d361ff868aaaafe1</anchor>
      <arglist>(DPM_Handle dpmHandle, DPM_InitCfg *ptrInitCfg, int32_t *errCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_execute</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga187686845f4e65efcf5a097cc8ec900b</anchor>
      <arglist>(DPM_DPCHandle handle, DPM_Buffer *ptrResult)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_ioctl</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga92bb16b39c0f3d93fff706f778af8db7</anchor>
      <arglist>(DPM_DPCHandle handle, uint32_t cmd, void *arg, uint32_t argLen)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_start</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga430131bb2f80c64112613e2c033a8ef1</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_stop</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaa17802063d07ea97b8904e959e41578c</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_deinit</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga5eab35ad6ea5c4c51c1b8e7b0650fc5a</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjectDetection_frameStart</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gabe06a45f022561d31c45c1ebb7f42857</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_DPC_Objdet_Assert</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>ab829cad7a9d82c2e96650a61af1605cf</anchor>
      <arglist>(DPM_Handle handle, int32_t expression, const char *file, int32_t line)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_quadFit</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga38a02e3d78ea2876b429001792e59a67</anchor>
      <arglist>(float *x, float *y, float *xv, float *yv)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_rangeBiasRxChPhaseMeasure</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gae36e17bc51634e4e4d155242f9d392da</anchor>
      <arglist>(DPC_ObjectDetection_StaticCfg *staticCfg, float targetDistance, float searchWinSize, uint16_t *detMatrix, uint32_t *symbolMatrix, DPU_AoAProc_compRxChannelBiasCfg *compRxChanCfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_GetRangeWinGenLen</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga6ac42f0ba06ff459cd4fb857e7856bee</anchor>
      <arglist>(DPU_RangeProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_GenRangeWindow</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2f463e6965f2ccf32cb5048bf38d33ac</anchor>
      <arglist>(DPU_RangeProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_GetDopplerWinGenLen</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab7a3959a7d938f23fd14a5fb7ffe6736</anchor>
      <arglist>(DPU_DopplerProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_GenDopplerWindow</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab1a6c01f7f111a19fb678a1fa9a19925</anchor>
      <arglist>(DPU_DopplerProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_GetRxChPhaseComp</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga0bdebcef1961a77d81c74a30b637bf12</anchor>
      <arglist>(DPC_ObjectDetection_StaticCfg *staticCfg, DPU_AoAProc_compRxChannelBiasCfg *inpCfg, DPU_AoAProc_compRxChannelBiasCfg *outCfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_GetAntGeometryDef</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaae62ddf31888bc4c8b39714ff29be57a</anchor>
      <arglist>(DPC_ObjectDetection_StaticCfg *staticCfg, ANTDEF_AntGeometry *antDef)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_elevFFTCfg</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a60727891f1cc2a7cb4dafd9377c1f78f</anchor>
      <arglist>(DPU_AoAProcHWA_StaticConfig *DPParams)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_reconfigSubFrame</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga24e306645f508fb57e631152a51da2b8</anchor>
      <arglist>(ObjDetObj *objDetObj, uint8_t subFrameIndx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>ObjectDetection_freeDmaChannels</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a53985a64e86a33e9ffc71b9837f8978f</anchor>
      <arglist>(EDMA_Handle edmaHandle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_Config_StaticClutterRemovalCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga9d7a3f11214c358ac88867152ef68590</anchor>
      <arglist>(SubFrameObj *obj, DPC_ObjectDetection_StaticClutterRemovalCfg_Base *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_Config_MeasureRxChannelBiasCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafcdcd265e54a01979e15c2788b645ba1</anchor>
      <arglist>(ObjDetObj *obj, DPC_ObjectDetection_MeasureRxChannelBiasCfg *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>allocateEDMAShadowChannel</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a1b001e3c816ae19d3d8621b076ad8248</anchor>
      <arglist>(EDMA_Handle edmaHandle, uint32_t *param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_rangeConfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gae3fc3fc1ce27cdb9da0c44cacc9a5115</anchor>
      <arglist>(DPU_RangeProcHWA_Handle dpuHandle, DPC_ObjectDetection_StaticCfg *staticCfg, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_RadarCube *radarCube, MemPoolObj *CoreLocalRamObj, uint32_t *windowOffset, uint32_t *CoreLocalRamScratchUsage, DPU_RangeProcHWA_Config *cfgSave, ObjDetObj *ptrObjDetObj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_dopplerConfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga3bf3f4c5c40c663f1ff6cca161112781</anchor>
      <arglist>(DPU_DopplerProcHWA_Handle dpuHandle, DPC_ObjectDetection_StaticCfg *staticCfg, uint8_t log2NumDopplerBins, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_RadarCube *radarCube, DPIF_DetMatrix *detMatrix, MemPoolObj *CoreLocalRamObj, uint32_t *windowOffset, uint32_t *CoreLocalRamScratchUsage, DPU_DopplerProcHWA_Config *cfgSave, ObjDetObj *ptrObjDetObj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_CFARconfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaeb1b8e25a6183b9f44bb5c7ec592452f</anchor>
      <arglist>(DPU_CFARProcHWA_Handle dpuHandle, DPC_ObjectDetection_StaticCfg *staticCfg, uint8_t log2NumDopplerBins, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_DetMatrix *detMatrix, DPIF_CFARDetList *cfarRngDopSnrList, uint32_t cfarRngDopSnrListSize, MemPoolObj *CoreLocalRamObj, uint32_t *hwaMemBankAddr, uint16_t hwaMemBankSize, float rangeBias, uint32_t *CoreLocalRamScratchUsage, DPU_CFARProcHWA_Config *cfgSave)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_AoAconfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gad852edfd433008fb9d112a74d25b62cb</anchor>
      <arglist>(DPU_AoAProcHWA_Handle dpuHandle, DPC_ObjectDetection_PreStartCommonCfg *commonCfg, DPC_ObjectDetection_StaticCfg *staticCfg, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_RadarCube *radarCube, DPIF_CFARDetList *cfarRngDopSnrList, uint32_t cfarRngDopSnrListSize, MemPoolObj *CoreLocalRamObj, MemPoolObj *L3RamObj, uint8_t dopplerWindowSym, uint32_t dopplerWinSize, int32_t *dopplerWindow, uint32_t dopplerWinRamOffset, uint8_t cfarParamSetStartIdx, bool *isAoAHWAparamSetOverlappedWithCFAR, DPU_AoAProcHWA_Config *cfgSave)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_preStartConfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2fcaabe01bfd966c5cdbf81361a1be6e</anchor>
      <arglist>(SubFrameObj *obj, DPC_ObjectDetection_PreStartCommonCfg *commonCfg, DPC_ObjectDetection_StaticCfg *staticCfg, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle[EDMA_NUM_CC], MemPoolObj *L3ramObj, MemPoolObj *CoreLocalRamObj, uint32_t *hwaMemBankAddr, uint16_t hwaMemBankSize, uint32_t *L3RamUsage, uint32_t *CoreLocalRamUsage, ObjDetObj *ptrObjDetObj)</arglist>
    </member>
    <member kind="variable">
      <type>ObjDetObj *</type>
      <name>gObjDetObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gabc921bdaf6a51cf6a6ec274a7995bac3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint8_t</type>
      <name>gObjectDetectionHeapMem</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>a9f1012acf8ccb0d090721c8e8b795b1e</anchor>
      <arglist>[OBJECTDETECTION_HEAP_MEM_SIZE]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static HeapP_Object</type>
      <name>gObjectDetectionHeapObj</name>
      <anchorfile>objectdetection_8c.html</anchorfile>
      <anchor>ab224f0b59c49345750ef913c024c8284</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPM_ProcChainCfg</type>
      <name>gDPC_ObjectDetectionCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____g_l_o_b_a_l.html</anchorfile>
      <anchor>ga281e7283fc81524fbf3f01d662b8ce8e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_AntGeometryCfg_t</name>
    <filename>struct_d_p_c___object_detection___ant_geometry_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>row</name>
      <anchorfile>struct_d_p_c___object_detection___ant_geometry_cfg__t.html</anchorfile>
      <anchor>a9c895344ae90545b230d36b28d15614c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>col</name>
      <anchorfile>struct_d_p_c___object_detection___ant_geometry_cfg__t.html</anchorfile>
      <anchor>adaaf7453d7c27c1c2d7953aa819c0e76</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_CalibDcRangeSigCfg_t</name>
    <filename>struct_d_p_c___object_detection___calib_dc_range_sig_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection___calib_dc_range_sig_cfg__t.html</anchorfile>
      <anchor>a990345fa3f88b79b2c0c23db003c36c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_RangeProc_CalibDcRangeSigCfg</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection___calib_dc_range_sig_cfg__t.html</anchorfile>
      <anchor>a72d7cb977af9a76cf4b3856d4d4545e8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_CfarCfg_t</name>
    <filename>struct_d_p_c___object_detection___cfar_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection___cfar_cfg__t.html</anchorfile>
      <anchor>ad777e3c644da52223aee66f4e14e1a81</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProc_CfarCfg</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection___cfar_cfg__t.html</anchorfile>
      <anchor>af7dc998d802177cc39706060af7de7c5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage_t</name>
    <filename>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>L3RamTotal</name>
      <anchorfile>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</anchorfile>
      <anchor>a922d9dad73df2340b01180dba92f908e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>L3RamUsage</name>
      <anchorfile>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</anchorfile>
      <anchor>ada536bd0d847ac895ade8c0fb803450f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>CoreLocalRamTotal</name>
      <anchorfile>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</anchorfile>
      <anchor>aca7994bb30de319b264944fd569818d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>CoreLocalRamUsage</name>
      <anchorfile>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</anchorfile>
      <anchor>a8e2a7ee84d41049b495160969191ddd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>SystemHeapTotal</name>
      <anchorfile>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</anchorfile>
      <anchor>a9d245e77ac211483a8aa1c623f30cb17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>SystemHeapUsed</name>
      <anchorfile>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</anchorfile>
      <anchor>ab095b27fad1b73cc90a5874b34756abe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>SystemHeapDPCUsed</name>
      <anchorfile>struct_d_p_c___object_detection___d_p_c___i_o_c_t_l__pre_start_cfg__mem_usage__t.html</anchorfile>
      <anchor>aacbfff63bb6774cd0260bccfcd0085a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_DynCfg_t</name>
    <filename>struct_d_p_c___object_detection___dyn_cfg__t.html</filename>
    <member kind="variable">
      <type>DPU_RangeProc_CalibDcRangeSigCfg</type>
      <name>calibDcRangeSigCfg</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>af4cb37302e7d000c3ef907edbc870470</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProc_CfarCfg</type>
      <name>cfarCfgRange</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>a4b0f1aa22b1dc7c6edfece7f733a9e31</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProc_CfarCfg</type>
      <name>cfarCfgDoppler</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>a44af3f81bde91cf06e0a916909b335c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProc_FovCfg</type>
      <name>fovRange</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>a736cd31ade4ad37e1f7d115476a8c945</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProc_FovCfg</type>
      <name>fovDoppler</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>a0a6439a1b60ce75b5ab4e520fa17d3f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_MultiObjBeamFormingCfg</type>
      <name>multiObjBeamFormingCfg</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>a007f7cba9016eca3abdfae0274a0e2aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>prepareRangeAzimuthHeatMap</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>ad63e5e63c0bc8eb1788e3a32adba9d84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_FovAoaCfg</type>
      <name>fovAoaCfg</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>ad98e83d4254e63cb69ae4efd83ec79b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_ExtendedMaxVelocityCfg</type>
      <name>extMaxVelCfg</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>a487353d71560f17e5b033a113377642d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_StaticClutterRemovalCfg_Base</type>
      <name>staticClutterRemovalCfg</name>
      <anchorfile>struct_d_p_c___object_detection___dyn_cfg__t.html</anchorfile>
      <anchor>afdeb7b99128d2d9d6dffbfbd21d94a88</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_ExecuteResult_t</name>
    <filename>struct_d_p_c___object_detection___execute_result__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameIdx</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>ade53dd18e414dbcfef46c3766ad660a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>numObjOut</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>ad77401edc625cd5e6f75e8f61cee74a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPIF_PointCloudCartesian *</type>
      <name>objOut</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>a80c85f7158a16dc16ad435ae23d7481d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPIF_RadarCube</type>
      <name>radarCube</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>a455b48c1b84b9aae9337e03ccadf76e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPIF_DetMatrix</type>
      <name>detMatrix</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>a7f2bde7397d236d1840301984072dd0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPIF_PointCloudSideInfo *</type>
      <name>objOutSideInfo</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>a9cc364121b00cea11269458657deac96</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>cmplx16ImRe_t *</type>
      <name>azimuthStaticHeatMap</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>a833095bd8c45533ec541bc90cd9475cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>azimuthStaticHeatMapSize</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>a6a709d0bc05944956987d9ae987b4a5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_Stats *</type>
      <name>stats</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>ac3b2d4528e7255d71759cd411e33d40b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_compRxChannelBiasCfg *</type>
      <name>compRxChanBiasMeasurement</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result__t.html</anchorfile>
      <anchor>a1d115a7fd41c8f3998611247d0c800a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_ExecuteResultExportedInfo_t</name>
    <filename>struct_d_p_c___object_detection___execute_result_exported_info__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameIdx</name>
      <anchorfile>struct_d_p_c___object_detection___execute_result_exported_info__t.html</anchorfile>
      <anchor>a1c21d233269b4257a388f97581dc06bd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_extMaxVelCfg_t</name>
    <filename>struct_d_p_c___object_detection__ext_max_vel_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection__ext_max_vel_cfg__t.html</anchorfile>
      <anchor>a2fa62fca6f74a06ed8b3db997923267f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_ExtendedMaxVelocityCfg</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection__ext_max_vel_cfg__t.html</anchorfile>
      <anchor>af9c1b502b7aab0d2e3db18c405cf0427</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_fovAoaCfg_t</name>
    <filename>struct_d_p_c___object_detection__fov_aoa_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection__fov_aoa_cfg__t.html</anchorfile>
      <anchor>a2883a6d087173d1bb2c9e2896528a719</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_FovAoaCfg</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection__fov_aoa_cfg__t.html</anchorfile>
      <anchor>a36341ac095f99217c780f1455d2ab04b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_fovDopplerCfg_t</name>
    <filename>struct_d_p_c___object_detection__fov_doppler_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection__fov_doppler_cfg__t.html</anchorfile>
      <anchor>ae124623707d63dd7339e60c4ab0e1282</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProc_FovCfg</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection__fov_doppler_cfg__t.html</anchorfile>
      <anchor>acba2b5a68208a5ea685eb316e78f790e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_fovRangeCfg_t</name>
    <filename>struct_d_p_c___object_detection__fov_range_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection__fov_range_cfg__t.html</anchorfile>
      <anchor>a7d77282ddfc06db15f7e528e87a3d886</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProc_FovCfg</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection__fov_range_cfg__t.html</anchorfile>
      <anchor>a6a7bab72bde6ffe3a05fb0f878f6c3e1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_InitParams_t</name>
    <filename>struct_d_p_c___object_detection___init_params__t.html</filename>
    <member kind="variable">
      <type>HWA_Handle</type>
      <name>hwaHandle</name>
      <anchorfile>struct_d_p_c___object_detection___init_params__t.html</anchorfile>
      <anchor>a7315cb85895ac6e3b8355cf5789f4c80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EDMA_Handle</type>
      <name>edmaHandle</name>
      <anchorfile>struct_d_p_c___object_detection___init_params__t.html</anchorfile>
      <anchor>a618820ad8fc9891620f4109b05ca09b7</anchor>
      <arglist>[EDMA_NUM_CC]</arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_MemCfg</type>
      <name>L3ramCfg</name>
      <anchorfile>struct_d_p_c___object_detection___init_params__t.html</anchorfile>
      <anchor>a747cb5f37bb064c1d8e21446f1010971</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_MemCfg</type>
      <name>CoreLocalRamCfg</name>
      <anchorfile>struct_d_p_c___object_detection___init_params__t.html</anchorfile>
      <anchor>a9a7a77461ef3647a4eceec71267e709e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_ProcessCallBackCfg</type>
      <name>processCallBackCfg</name>
      <anchorfile>struct_d_p_c___object_detection___init_params__t.html</anchorfile>
      <anchor>ad4f850bf48550cd7cc2fc0209e222810</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_MeasureRxChannelBiasCfg_t</name>
    <filename>struct_d_p_c___object_detection___measure_rx_channel_bias_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>enabled</name>
      <anchorfile>struct_d_p_c___object_detection___measure_rx_channel_bias_cfg__t.html</anchorfile>
      <anchor>a243bd494352d2138f1e01271810e1373</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>targetDistance</name>
      <anchorfile>struct_d_p_c___object_detection___measure_rx_channel_bias_cfg__t.html</anchorfile>
      <anchor>a7c264c66c0912800939ac2dc821323c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>searchWinSize</name>
      <anchorfile>struct_d_p_c___object_detection___measure_rx_channel_bias_cfg__t.html</anchorfile>
      <anchor>ac3a21a07d41a24d7ba511b50d310d64d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_MemCfg_t</name>
    <filename>struct_d_p_c___object_detection___mem_cfg__t.html</filename>
    <member kind="variable">
      <type>void *</type>
      <name>addr</name>
      <anchorfile>struct_d_p_c___object_detection___mem_cfg__t.html</anchorfile>
      <anchor>a30d708e2aebdd86dd789062f608a159f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>size</name>
      <anchorfile>struct_d_p_c___object_detection___mem_cfg__t.html</anchorfile>
      <anchor>a888ad04d48d28b4a24910ab93b760049</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_MultiObjBeamFormingCfg_t</name>
    <filename>struct_d_p_c___object_detection___multi_obj_beam_forming_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection___multi_obj_beam_forming_cfg__t.html</anchorfile>
      <anchor>a63177bce3e25c3789b7d98912e784df9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_MultiObjBeamFormingCfg</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection___multi_obj_beam_forming_cfg__t.html</anchorfile>
      <anchor>a3c341b61918596b9a91adf108528d36c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_PreStartCfg_t</name>
    <filename>struct_d_p_c___object_detection___pre_start_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_cfg__t.html</anchorfile>
      <anchor>a84dda582b9148ce0d91d55e3ac4f369c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_StaticCfg</type>
      <name>staticCfg</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_cfg__t.html</anchorfile>
      <anchor>a25e254ae0f68bcd7d0438c83bd42a39d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_DynCfg</type>
      <name>dynCfg</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_cfg__t.html</anchorfile>
      <anchor>ac8a96481a5524dd2966821341ac04527</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage</type>
      <name>memUsage</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_cfg__t.html</anchorfile>
      <anchor>a8802b99c48dfb2c871149e307bb27f1e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_PreStartCommonCfg_t</name>
    <filename>struct_d_p_c___object_detection___pre_start_common_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numSubFrames</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_common_cfg__t.html</anchorfile>
      <anchor>a1009b709c1fa3f15f2c3d93580734ae5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_MeasureRxChannelBiasCfg</type>
      <name>measureRxChannelBiasCfg</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_common_cfg__t.html</anchorfile>
      <anchor>a6bc589fcc17d780889d452025fa6ee82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_compRxChannelBiasCfg</type>
      <name>compRxChanCfg</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_common_cfg__t.html</anchorfile>
      <anchor>a2e483e5e8979be44d7ed778369a3c672</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ANTDEF_AntGeometry</type>
      <name>antDef</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_common_cfg__t.html</anchorfile>
      <anchor>ada8661ff8a3f89d7fbac329def59ead2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_AntGeometryCfg</type>
      <name>antGeometryCfg</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_common_cfg__t.html</anchorfile>
      <anchor>a44e2a5fb7d0d5931b73833d19b8d69cf</anchor>
      <arglist>[SYS_COMMON_NUM_RX_CHANNEL *SYS_COMMON_NUM_TX_ANTENNAS]</arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProcHWA_AntennaSpacing</type>
      <name>antennaSpacing</name>
      <anchorfile>struct_d_p_c___object_detection___pre_start_common_cfg__t.html</anchorfile>
      <anchor>af6c38a4420bb9cf55138d05721e9036c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_ProcessCallBackCfg_t</name>
    <filename>struct_d_p_c___object_detection___process_call_back_cfg__t.html</filename>
    <member kind="variable">
      <type>DPC_ObjectDetection_processCallBackFxn_t</type>
      <name>processFrameBeginCallBackFxn</name>
      <anchorfile>struct_d_p_c___object_detection___process_call_back_cfg__t.html</anchorfile>
      <anchor>a0f865303e278b50d832cb0892d2ce4bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_processCallBackFxn_t</type>
      <name>processInterFrameBeginCallBackFxn</name>
      <anchorfile>struct_d_p_c___object_detection___process_call_back_cfg__t.html</anchorfile>
      <anchor>ad16e510aab7cebd37db054fcd9d4ba92</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_RangeAzimuthHeatMapCfg_t</name>
    <filename>struct_d_p_c___object_detection___range_azimuth_heat_map_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection___range_azimuth_heat_map_cfg__t.html</anchorfile>
      <anchor>a3bdd5848cde0ea6cfb89d71040ec3047</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>prepareRangeAzimuthHeatMap</name>
      <anchorfile>struct_d_p_c___object_detection___range_azimuth_heat_map_cfg__t.html</anchorfile>
      <anchor>aacd6958e5eba65ccab5536d63e4f1bef</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_StaticCfg_t</name>
    <filename>struct_d_p_c___object_detection___static_cfg__t.html</filename>
    <member kind="variable">
      <type>DPIF_ADCBufData</type>
      <name>ADCBufData</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a4ab9d63a4b22ae6d9fce208192bba8f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>rxAntOrder</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a275a488a00db72b336b0e50910e6d048</anchor>
      <arglist>[SYS_COMMON_NUM_RX_CHANNEL]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>txAntOrder</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>ac2c2a814681a583213d683fd53d0a547</anchor>
      <arglist>[SYS_COMMON_NUM_TX_ANTENNAS]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numTxAntennas</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a8d21d71697ef18326ba8f30be4c90464</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numVirtualAntennas</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a74d4345a14df74d75ef3c01f6a36320d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numVirtualAntAzim</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>af7196c0a99916b6e54e577e87a0fc9d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>numVirtualAntElev</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a1ece8ded3f01580f06d305c62d2a73f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>numRangeBins</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a5a8313ab6a758c134766718da2b4442d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>numRangeFFTBins</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a719bee9adfa75bc342dac4cd185fe0c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>numChirpsPerFrame</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a1070837315fa7998816f740e9b42e5fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>numDopplerChirps</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a6569cc59cc8829a973e231bbf0219bc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>numDopplerBins</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a46feb165c1a9e3c666fb343babd789d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>rangeStep</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a254e5c0d9ed582543160e708b14bcc4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>dopplerStep</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a676d47edfd62a07adb80b0e29a60e58f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>isValidProfileHasOneTxPerChirp</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a6a58639bc54dc8279a53ac895356abeb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ANTDEF_AntGeometry</type>
      <name>antDef</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>ad532c828a27d9822e2928ee9d3555bc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_RangeProcHWA_FFTtuning</type>
      <name>rangeFFTtuning</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>aed84e5b41ac5736fd0b88ba79b6bae0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_RangeProcHWA_InputMode</type>
      <name>dataInputMode</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>ada968e9a9be7a98d60ef8928882be898</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Float32</type>
      <name>framePeriod</name>
      <anchorfile>struct_d_p_c___object_detection___static_cfg__t.html</anchorfile>
      <anchor>a6a44b6e13e7bfd76d0d45faf0fe1f709</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_StaticClutterRemovalCfg_Base_t</name>
    <filename>struct_d_p_c___object_detection___static_clutter_removal_cfg___base__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>enabled</name>
      <anchorfile>struct_d_p_c___object_detection___static_clutter_removal_cfg___base__t.html</anchorfile>
      <anchor>ac7a4a11579ac33f7f80f7bb276e5a99c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_StaticClutterRemovalCfg_t</name>
    <filename>struct_d_p_c___object_detection___static_clutter_removal_cfg__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameNum</name>
      <anchorfile>struct_d_p_c___object_detection___static_clutter_removal_cfg__t.html</anchorfile>
      <anchor>adc4be2ed0c02e871bdffbf2660fb9972</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_StaticClutterRemovalCfg_Base</type>
      <name>cfg</name>
      <anchorfile>struct_d_p_c___object_detection___static_clutter_removal_cfg__t.html</anchorfile>
      <anchor>a92b00ba61a35c504d1b9ad990758db8c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DPC_ObjectDetection_Stats_t</name>
    <filename>struct_d_p_c___object_detection___stats__t.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>interChirpProcessingMargin</name>
      <anchorfile>struct_d_p_c___object_detection___stats__t.html</anchorfile>
      <anchor>a59731b92e13b173e5e107455ff3ace96</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>frameStartIntCounter</name>
      <anchorfile>struct_d_p_c___object_detection___stats__t.html</anchorfile>
      <anchor>af9026407036f24c134af5cb70e57e0aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>frameStartTimeStamp</name>
      <anchorfile>struct_d_p_c___object_detection___stats__t.html</anchorfile>
      <anchor>af7bc72e0b5bb8732e652a0591478bb37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>interFrameStartTimeStamp</name>
      <anchorfile>struct_d_p_c___object_detection___stats__t.html</anchorfile>
      <anchor>aa6f63054bb5ec301644aa87d6166bb71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>interFrameEndTimeStamp</name>
      <anchorfile>struct_d_p_c___object_detection___stats__t.html</anchorfile>
      <anchor>a277dc10aeb5ad8b4e8be8974f3a14ffb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>subFramePreparationCycles</name>
      <anchorfile>struct_d_p_c___object_detection___stats__t.html</anchorfile>
      <anchor>a65c8f0e1d8c390c5dfb3c728dfd5ffd6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DpuConfigs_t</name>
    <filename>struct_dpu_configs__t.html</filename>
    <member kind="variable">
      <type>DPU_RangeProcHWA_Config</type>
      <name>rangeCfg</name>
      <anchorfile>struct_dpu_configs__t.html</anchorfile>
      <anchor>a8bfeafbd90c0d00159d03db65d389fb6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_DopplerProcHWA_Config</type>
      <name>dopplerCfg</name>
      <anchorfile>struct_dpu_configs__t.html</anchorfile>
      <anchor>ae07813fc7c9a62f145819e329e96f826</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProcHWA_Config</type>
      <name>cfarCfg</name>
      <anchorfile>struct_dpu_configs__t.html</anchorfile>
      <anchor>a5fb05bfb2e27dd0d9ed7f9ed8a957ffc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProcHWA_Config</type>
      <name>aoaCfg</name>
      <anchorfile>struct_dpu_configs__t.html</anchorfile>
      <anchor>a8db7cf329c045d48ed52f334fb075524</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Element_t</name>
    <filename>struct_element__t.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>value</name>
      <anchorfile>struct_element__t.html</anchorfile>
      <anchor>a6e75b3747e33ebb1c21b03e97add3a0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>index</name>
      <anchorfile>struct_element__t.html</anchorfile>
      <anchor>a822109a5ac5f5220c5c20f1198db8abe</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MemPoolObj_t</name>
    <filename>struct_mem_pool_obj__t.html</filename>
    <member kind="variable">
      <type>DPC_ObjectDetection_MemCfg</type>
      <name>cfg</name>
      <anchorfile>struct_mem_pool_obj__t.html</anchorfile>
      <anchor>affa2add93a603eca2a08a4d2729783e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uintptr_t</type>
      <name>currAddr</name>
      <anchorfile>struct_mem_pool_obj__t.html</anchorfile>
      <anchor>aed53f855d3513464e550473628870598</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uintptr_t</type>
      <name>maxCurrAddr</name>
      <anchorfile>struct_mem_pool_obj__t.html</anchorfile>
      <anchor>af8af6e3a9262553834f6a471df4052f1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ObjDetObj_t</name>
    <filename>struct_obj_det_obj__t.html</filename>
    <member kind="variable">
      <type>DPM_InitCfg</type>
      <name>dpmInitCfg</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a957be053d570f6e0b9efabf20509ab1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPM_Handle</type>
      <name>dpmHandle</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>ae7009786f983d03ce4ee88e3b54cfbfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SubFrameObj</type>
      <name>subFrameObj</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>ae24b03698f46653f9a5541f609d31cf0</anchor>
      <arglist>[RL_MAX_SUBFRAMES]</arglist>
    </member>
    <member kind="variable">
      <type>Edma_IntrObject</type>
      <name>rangProcIntrObj</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a3cd5f6192e0bcc9e68c24738d0ff080a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Edma_IntrObject</type>
      <name>dopplerProcIntrObj</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a097f13d1675726498a5a08fa5cbee933</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subFrameIndx</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a79f152b69c5e07330bab89befde5d402</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EDMA_Handle</type>
      <name>edmaHandle</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>afb73f95ee4cc83f378be37cb289482da</anchor>
      <arglist>[EDMA_NUM_CC]</arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>interSubFrameProcToken</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>adc053c8d84639770859b960ebf1a4af9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MemPoolObj</type>
      <name>L3RamObj</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>ae824942cea577d75b493a02a3e85c00d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MemPoolObj</type>
      <name>CoreLocalRamObj</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a26699db9fa02d21feb0498174c057147</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>hwaMemBankAddr</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>ab24c817f483190d607b499674b1710db</anchor>
      <arglist>[SOC_HWA_NUM_MEM_BANKS]</arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>hwaMemBankSize</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a26ddd6ac2126d804128b90c026e803fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_PreStartCommonCfg</type>
      <name>commonCfg</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a6d7a86e1bf1e2f0916318723453897c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isCommonCfgReceived</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a38c04d66fcfaa4c85cc99f42bfddbaa2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_ExecuteResult</type>
      <name>executeResult</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>ad89a1a6bc97dfca73ce902f786d055e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProc_compRxChannelBiasCfg</type>
      <name>compRxChanCfgMeasureOut</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a8efc71e4c5fa26f911bb33ef014a469f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_Stats</type>
      <name>stats</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>a99ba4c27c505996ffd0c5340e50ad4c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_ProcessCallBackCfg</type>
      <name>processCallBackCfg</name>
      <anchorfile>struct_obj_det_obj__t.html</anchorfile>
      <anchor>ab882a6fe7af604349ae737ec7a828e8c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SubFrameObj_t</name>
    <filename>struct_sub_frame_obj__t.html</filename>
    <member kind="variable">
      <type>DPU_RangeProcHWA_Handle</type>
      <name>dpuRangeObj</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a80b9bd323a141636d9ae990e8409e389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_CFARProcHWA_Handle</type>
      <name>dpuCFARObj</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a8d405aa9ebf954ed8b68961f19add1dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_DopplerProcHWA_Handle</type>
      <name>dpuDopplerObj</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a505a6a65f68688cb4e4e5ba50ccba20d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPU_AoAProcHWA_Handle</type>
      <name>dpuAoAObj</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a56b0b02acb9fcdec290bd10279691678</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_StaticCfg</type>
      <name>staticCfg</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>aba389f7f6362bb018619742cb4cf5677</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPC_ObjectDetection_DynCfg</type>
      <name>dynCfg</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a38b9e73ebf0b34a3c96a30d8b9f9df6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>log2NumDopplerBins</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a96b75e8aa234e89593b6a210b20f57f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DpuConfigs</type>
      <name>dpuCfg</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a37653d9d2f77b2b0d30203bf759dc483</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isAoAHWAparamSetOverlappedWithCFAR</name>
      <anchorfile>struct_sub_frame_obj__t.html</anchorfile>
      <anchor>a1256ac0cab25ef3ed1af8418436f1e7b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET_INTERNAL</name>
    <title>Object Detection DPC (Data Path Chain) Internal</title>
    <filename>group___d_p_c___o_b_j_d_e_t___i_n_t_e_r_n_a_l.html</filename>
    <subgroup>DPC_OBJDET_IOCTL__INTERNAL_DATA_STRUCTURES</subgroup>
    <subgroup>DPC_OBJDET_IOCTL__INTERNAL_DEFINITIONS</subgroup>
    <subgroup>DPC_OBJDET__INTERNAL_FUNCTION</subgroup>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET_IOCTL__INTERNAL_DATA_STRUCTURES</name>
    <title>Object Detection DPC Internal Data Structures</title>
    <filename>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</filename>
    <class kind="struct">DpuConfigs_t</class>
    <class kind="struct">SubFrameObj_t</class>
    <class kind="struct">MemPoolObj_t</class>
    <class kind="struct">Element_t</class>
    <class kind="struct">ObjDetObj_t</class>
    <member kind="typedef">
      <type>struct DpuConfigs_t</type>
      <name>DpuConfigs</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gacba76ea4873414351f89cbae8b152002</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct SubFrameObj_t</type>
      <name>SubFrameObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga66223d3fe88a478232a4baa67bdb5920</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct MemPoolObj_t</type>
      <name>MemPoolObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga788ff8f9775b0058d4fbaea936649d68</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct Element_t</type>
      <name>Element</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga57b5a60faaa35f7409b5d1b9828e7adc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct ObjDetObj_t</type>
      <name>ObjDetObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga840cbb1d253d01785da21bc735411d6f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET_IOCTL__INTERNAL_DEFINITIONS</name>
    <title>Object Detection DPC Internal Definitions</title>
    <filename>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>DPC_HWA_MEM_BANK_INDX_CFARDETMAT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gad3e7dbba6defd511fbdc2327460bbf76</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_HWA_MEM_BANK_INDX_DOPPLEROUT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga810bba96d82ffaa5c7eec631f58c3796</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_HWA_MEM_BANK_INDX_RANGEOUT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gad3d494006d06a49d23f841c72b386782</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gaa8731c03d10d2315e5eadb7f2afd53ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_DET_MATRIX_DATABUF_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga638d914fc7dfb01a653d8d4bbf805251</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gac59d123e24422d9bbfeede1667d53d4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gaeb63c1b2a34f840577a8aebf7b1008ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_CFAR_DET_LIST_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga501d3496615416c113324d0630e752e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gac27e9457ac05b7b1cca50df75707c8a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gab485b4470921a17dcd3cba2af9a65710</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga4cc0b43c12df53c1516a7d6fe59647ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga4950fd5a472db2110b7a35b5bcd047c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gaae738fb0a93d1f1b3a4c1f8826a853bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga0d160f044d6b27d08190840b2cd01860</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gad29d677b3c66671d1653f2e721795013</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga23750a6aa6cb1454bf77d745edb2f4be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ObjDetObj *</type>
      <name>gObjDetObj</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____i_n_t_e_r_n_a_l___d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gabc921bdaf6a51cf6a6ec274a7995bac3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET__INTERNAL_FUNCTION</name>
    <title>Object Detection DPC Internal Functions</title>
    <filename>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_MemPoolReset</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafa164e3ed6ac64da116723deae8bb2b4</anchor>
      <arglist>(MemPoolObj *pool)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_MemPoolSet</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga82c3045ac8fafd9a6c1ba20438550c7c</anchor>
      <arglist>(MemPoolObj *pool, void *addr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>DPC_ObjDet_MemPoolGet</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga58cd28e5cd493d5e8a8a320469ca45a1</anchor>
      <arglist>(MemPoolObj *pool)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_MemPoolGetMaxUsage</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga058866af95b1947003f708af8a31871e</anchor>
      <arglist>(MemPoolObj *pool)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void *</type>
      <name>DPC_ObjDet_MemPoolAlloc</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2513799b7ea5d07a2d79444e05506ae1</anchor>
      <arglist>(MemPoolObj *pool, uint32_t size, uint8_t align)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjectDetection_frameStart</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gabe06a45f022561d31c45c1ebb7f42857</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_quadFit</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga38a02e3d78ea2876b429001792e59a67</anchor>
      <arglist>(float *x, float *y, float *xv, float *yv)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_rangeBiasRxChPhaseMeasure</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gae36e17bc51634e4e4d155242f9d392da</anchor>
      <arglist>(DPC_ObjectDetection_StaticCfg *staticCfg, float targetDistance, float searchWinSize, uint16_t *detMatrix, uint32_t *symbolMatrix, DPU_AoAProc_compRxChannelBiasCfg *compRxChanCfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_GetRangeWinGenLen</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga6ac42f0ba06ff459cd4fb857e7856bee</anchor>
      <arglist>(DPU_RangeProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_GenRangeWindow</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2f463e6965f2ccf32cb5048bf38d33ac</anchor>
      <arglist>(DPU_RangeProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_GetDopplerWinGenLen</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab7a3959a7d938f23fd14a5fb7ffe6736</anchor>
      <arglist>(DPU_DopplerProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>DPC_ObjDet_GenDopplerWindow</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab1a6c01f7f111a19fb678a1fa9a19925</anchor>
      <arglist>(DPU_DopplerProcHWA_Config *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_GetRxChPhaseComp</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga0bdebcef1961a77d81c74a30b637bf12</anchor>
      <arglist>(DPC_ObjectDetection_StaticCfg *staticCfg, DPU_AoAProc_compRxChannelBiasCfg *inpCfg, DPU_AoAProc_compRxChannelBiasCfg *outCfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_GetAntGeometryDef</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaae62ddf31888bc4c8b39714ff29be57a</anchor>
      <arglist>(DPC_ObjectDetection_StaticCfg *staticCfg, ANTDEF_AntGeometry *antDef)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_execute</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga187686845f4e65efcf5a097cc8ec900b</anchor>
      <arglist>(DPM_DPCHandle handle, DPM_Buffer *ptrResult)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_reconfigSubFrame</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga24e306645f508fb57e631152a51da2b8</anchor>
      <arglist>(ObjDetObj *objDetObj, uint8_t subFrameIndx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_start</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga430131bb2f80c64112613e2c033a8ef1</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_stop</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaa17802063d07ea97b8904e959e41578c</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>DPC_ObjDet_Config_StaticClutterRemovalCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga9d7a3f11214c358ac88867152ef68590</anchor>
      <arglist>(SubFrameObj *obj, DPC_ObjectDetection_StaticClutterRemovalCfg_Base *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_Config_MeasureRxChannelBiasCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gafcdcd265e54a01979e15c2788b645ba1</anchor>
      <arglist>(ObjDetObj *obj, DPC_ObjectDetection_MeasureRxChannelBiasCfg *cfg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_rangeConfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gae3fc3fc1ce27cdb9da0c44cacc9a5115</anchor>
      <arglist>(DPU_RangeProcHWA_Handle dpuHandle, DPC_ObjectDetection_StaticCfg *staticCfg, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_RadarCube *radarCube, MemPoolObj *CoreLocalRamObj, uint32_t *windowOffset, uint32_t *CoreLocalRamScratchUsage, DPU_RangeProcHWA_Config *cfgSave, ObjDetObj *ptrObjDetObj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_dopplerConfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga3bf3f4c5c40c663f1ff6cca161112781</anchor>
      <arglist>(DPU_DopplerProcHWA_Handle dpuHandle, DPC_ObjectDetection_StaticCfg *staticCfg, uint8_t log2NumDopplerBins, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_RadarCube *radarCube, DPIF_DetMatrix *detMatrix, MemPoolObj *CoreLocalRamObj, uint32_t *windowOffset, uint32_t *CoreLocalRamScratchUsage, DPU_DopplerProcHWA_Config *cfgSave, ObjDetObj *ptrObjDetObj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_CFARconfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gaeb1b8e25a6183b9f44bb5c7ec592452f</anchor>
      <arglist>(DPU_CFARProcHWA_Handle dpuHandle, DPC_ObjectDetection_StaticCfg *staticCfg, uint8_t log2NumDopplerBins, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_DetMatrix *detMatrix, DPIF_CFARDetList *cfarRngDopSnrList, uint32_t cfarRngDopSnrListSize, MemPoolObj *CoreLocalRamObj, uint32_t *hwaMemBankAddr, uint16_t hwaMemBankSize, float rangeBias, uint32_t *CoreLocalRamScratchUsage, DPU_CFARProcHWA_Config *cfgSave)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_AoAconfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gad852edfd433008fb9d112a74d25b62cb</anchor>
      <arglist>(DPU_AoAProcHWA_Handle dpuHandle, DPC_ObjectDetection_PreStartCommonCfg *commonCfg, DPC_ObjectDetection_StaticCfg *staticCfg, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle, DPIF_RadarCube *radarCube, DPIF_CFARDetList *cfarRngDopSnrList, uint32_t cfarRngDopSnrListSize, MemPoolObj *CoreLocalRamObj, MemPoolObj *L3RamObj, uint8_t dopplerWindowSym, uint32_t dopplerWinSize, int32_t *dopplerWindow, uint32_t dopplerWinRamOffset, uint8_t cfarParamSetStartIdx, bool *isAoAHWAparamSetOverlappedWithCFAR, DPU_AoAProcHWA_Config *cfgSave)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjDet_preStartConfig</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga2fcaabe01bfd966c5cdbf81361a1be6e</anchor>
      <arglist>(SubFrameObj *obj, DPC_ObjectDetection_PreStartCommonCfg *commonCfg, DPC_ObjectDetection_StaticCfg *staticCfg, DPC_ObjectDetection_DynCfg *dynCfg, EDMA_Handle edmaHandle[EDMA_NUM_CC], MemPoolObj *L3ramObj, MemPoolObj *CoreLocalRamObj, uint32_t *hwaMemBankAddr, uint16_t hwaMemBankSize, uint32_t *L3RamUsage, uint32_t *CoreLocalRamUsage, ObjDetObj *ptrObjDetObj)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_ioctl</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga92bb16b39c0f3d93fff706f778af8db7</anchor>
      <arglist>(DPM_DPCHandle handle, uint32_t cmd, void *arg, uint32_t argLen)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DPM_DPCHandle</type>
      <name>DPC_ObjectDetection_init</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>gab3688b470b396b17d361ff868aaaafe1</anchor>
      <arglist>(DPM_Handle dpmHandle, DPM_InitCfg *ptrInitCfg, int32_t *errCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>DPC_ObjectDetection_deinit</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____i_n_t_e_r_n_a_l___f_u_n_c_t_i_o_n.html</anchorfile>
      <anchor>ga5eab35ad6ea5c4c51c1b8e7b0650fc5a</anchor>
      <arglist>(DPM_DPCHandle handle)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET_EXTERNAL</name>
    <title>Object Detection DPC (Data-path Processing Chain) External</title>
    <filename>group___d_p_c___o_b_j_d_e_t___e_x_t_e_r_n_a_l.html</filename>
    <subgroup>DPC_OBJDET__GLOBAL</subgroup>
    <subgroup>DPC_OBJDET_IOCTL__DATA_STRUCTURES</subgroup>
    <subgroup>DPC_OBJDET_IOCTL__DEFINITIONS</subgroup>
    <subgroup>DPC_OBJDET_IOCTL__COMMAND</subgroup>
    <subgroup>DPC_OBJECTDETECTION_ERROR_CODE</subgroup>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET__GLOBAL</name>
    <title>Object Detection DPC Globals</title>
    <filename>group___d_p_c___o_b_j_d_e_t_____g_l_o_b_a_l.html</filename>
    <member kind="variable">
      <type>DPM_ProcChainCfg</type>
      <name>gDPC_ObjectDetectionCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____g_l_o_b_a_l.html</anchorfile>
      <anchor>ga281e7283fc81524fbf3f01d662b8ce8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DPM_ProcChainCfg</type>
      <name>gDPC_ObjectDetectionCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t_____g_l_o_b_a_l.html</anchorfile>
      <anchor>ga281e7283fc81524fbf3f01d662b8ce8e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET_IOCTL__DATA_STRUCTURES</name>
    <title>Object Detection DPC Data Structures</title>
    <filename>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</filename>
    <class kind="struct">DPC_ObjectDetection_CalibDcRangeSigCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_CfarCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_fovRangeCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_fovDopplerCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_MultiObjBeamFormingCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_fovAoaCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_extMaxVelCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_RangeAzimuthHeatMapCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_StaticClutterRemovalCfg_Base_t</class>
    <class kind="struct">DPC_ObjectDetection_StaticClutterRemovalCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_MeasureRxChannelBiasCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_ProcessCallBackCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_MemCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_InitParams_t</class>
    <class kind="struct">DPC_ObjectDetection_StaticCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_DynCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_AntGeometryCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_PreStartCommonCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage_t</class>
    <class kind="struct">DPC_ObjectDetection_PreStartCfg_t</class>
    <class kind="struct">DPC_ObjectDetection_Stats_t</class>
    <class kind="struct">DPC_ObjectDetection_ExecuteResult_t</class>
    <class kind="struct">DPC_ObjectDetection_ExecuteResultExportedInfo_t</class>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_CalibDcRangeSigCfg_t</type>
      <name>DPC_ObjectDetection_CalibDcRangeSigCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga8da4ca6327faacf817ac462fb5e45b71</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_CfarCfg_t</type>
      <name>DPC_ObjectDetection_CfarCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga653015b26a136a110c07d1b850dce32a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_fovRangeCfg_t</type>
      <name>DPC_ObjectDetection_fovRangeCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gac766a299ef497d723cdbaf5124a253ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_fovDopplerCfg_t</type>
      <name>DPC_ObjectDetection_fovDopplerCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga20922195965064877ab2e557b7cb5f99</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_MultiObjBeamFormingCfg_t</type>
      <name>DPC_ObjectDetection_MultiObjBeamFormingCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gad6d7fa935fbf961705171572d129dcbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_fovAoaCfg_t</type>
      <name>DPC_ObjectDetection_fovAoaCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gabf3e11f4cf8f677ad7dcd5cc8994af63</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_extMaxVelCfg_t</type>
      <name>DPC_ObjectDetection_extMaxVelCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gacd0db685da0e1167c27b1edc515dcd61</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_RangeAzimuthHeatMapCfg_t</type>
      <name>DPC_ObjectDetection_RangeAzimuthHeatMapCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga6f58b7e08d774e961c6feef3a68cea92</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_StaticClutterRemovalCfg_Base_t</type>
      <name>DPC_ObjectDetection_StaticClutterRemovalCfg_Base</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gadff964d923ee4adcdbeada10546789d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_StaticClutterRemovalCfg_t</type>
      <name>DPC_ObjectDetection_StaticClutterRemovalCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gaae0ac9d7b5d6a5043f0db5f88179f6b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_MeasureRxChannelBiasCfg_t</type>
      <name>DPC_ObjectDetection_MeasureRxChannelBiasCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga74947625bac7079aabc9008a3f32428f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>DPC_ObjectDetection_processCallBackFxn_t</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gab0ee2272c61cf46d095887deb89fac29</anchor>
      <arglist>)(uint8_t subFrameIndx)</arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_ProcessCallBackCfg_t</type>
      <name>DPC_ObjectDetection_ProcessCallBackCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gac692d02ace5b44eda283ac29d4d40682</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_MemCfg_t</type>
      <name>DPC_ObjectDetection_MemCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gad7eb47cceeb01abde75cfa49af110423</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_InitParams_t</type>
      <name>DPC_ObjectDetection_InitParams</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gaeb2ab2c35a97b750b34471f7ca9da638</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_StaticCfg_t</type>
      <name>DPC_ObjectDetection_StaticCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga204815ebe03bd22ddde993e03334cf7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_DynCfg_t</type>
      <name>DPC_ObjectDetection_DynCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga1d7e14014463d4d2074956cf8204255e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_AntGeometryCfg_t</type>
      <name>DPC_ObjectDetection_AntGeometryCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga7b27bdfa37c0f7e6089220e20bb587c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_PreStartCommonCfg_t</type>
      <name>DPC_ObjectDetection_PreStartCommonCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga525ff01906976598321804be6ba28e9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage_t</type>
      <name>DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gadf33cf6e1cd3a839847343333bfd9861</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_PreStartCfg_t</type>
      <name>DPC_ObjectDetection_PreStartCfg</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gad708887a74b45cd55835431170ab82cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_Stats_t</type>
      <name>DPC_ObjectDetection_Stats</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>ga1c01f9da6e20ecb450747a8bdd1bd0bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_ExecuteResult_t</type>
      <name>DPC_ObjectDetection_ExecuteResult</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gaf6613c3cafe1d95c280c1d603a4d4a52</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct DPC_ObjectDetection_ExecuteResultExportedInfo_t</type>
      <name>DPC_ObjectDetection_ExecuteResultExportedInfo</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_a_t_a___s_t_r_u_c_t_u_r_e_s.html</anchorfile>
      <anchor>gac86cb9157a5e88b81f5b22410988003d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET_IOCTL__DEFINITIONS</name>
    <title>Object Detection DPC Definitions</title>
    <filename>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_MAX_NUM_OBJECTS</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>gacc21bf75d7659447df554afa64ae0bc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUM_AZIM_VIRT_ANT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga7d33188163ed5a9de2d22fca4771289c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUM_ELEV_VIRT_ANT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga14a30791bb4128a5cd1e47d983b7f7f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NUM_VIRT_ANT</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____d_e_f_i_n_i_t_i_o_n_s.html</anchorfile>
      <anchor>ga9cc5752d3b3201b098ff53ee8e6eb772</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJDET_IOCTL__COMMAND</name>
    <title>Object Detection DPC Configuration Commands</title>
    <filename>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga80aa6076fb941f1be8943b04e7e91446</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gadd76b7822011f2beb6aa4ee56e34f1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_CFAR_RANGE_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaec9f74f65466642cc9bfaff59ec80e62</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_CFAR_DOPPLER_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga30e55ff74fd577e375da3f82332d373d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_MULTI_OBJ_BEAM_FORM_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaf6d697c2f90323ec9fad31e8a554d997</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_CALIB_DC_RANGE_SIG_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga2fa5ab168b3acb96f5bbe954330598e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_STATICCLUTTER_REMOVAL_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaaf6c32ded08c76e04a30518b72892952</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga2c431f3f05cc138597dfdfc2ce33e49a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga41ce76942b5dee5914dff7e84a7f0a19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_FOV_RANGE</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gaf5ebb36e5c52b6286fef0723a4b0a74f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_FOV_DOPPLER</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga4129c2bab5b30119da5d7491d94ce9b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_FOV_AOA</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga253239459a22f88595b927659a9fc6f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_RANGE_AZIMUTH_HEAT_MAP</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gadf9c7fa80f67d74bd1d7fe4024698a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_EXT_MAX_VELOCITY</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gab22059e0bec77c31a6806be1568127a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>gad16a4accd6829309809e215b7861ed4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJDET_IOCTL__TRIGGER_FRAME</name>
      <anchorfile>group___d_p_c___o_b_j_d_e_t___i_o_c_t_l_____c_o_m_m_a_n_d.html</anchorfile>
      <anchor>ga748dcbeed295626c339215deb7dfe627</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>DPC_OBJECTDETECTION_ERROR_CODE</name>
    <title>Object Detection DPC Error Codes</title>
    <filename>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga44b9e9ec1ff5cb005f2ef4a12e28ae73</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL__INIT_CFG_ARGSIZE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga6db2e9f59bdeeda09b4573885b267ea0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL__MEASURE_RX_CHANNEL_BIAS_CFG</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga29f7b672f9b0b0446108baf94957db87</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL__COMMAND</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga96cac2f23d951ac189e31070f9959295</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINVAL_ANT_PATTERN</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gac3f1655b8ba3c74612b18899eb1ffcad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga03456a240ccbe4b83bb6439d563acf48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__L3_RAM_RADAR_CUBE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gaf15f666e2ba42dc9b57f18886394ec96</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__L3_RAM_DET_MATRIX</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga21fbd3e8a1a008b2496a696929a5b38b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM_HWA_WINDOW_RAM</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gade825cdfa1c1ba9d3f739a7f0eeabd86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga16c37bf87fb62f456b36579664fb853e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_HWA_WINDOW</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gac7cb577287bc37fe3cb8dad12c97c003</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_CFAR_DOPPLER_DET_OUT_BIT_MASK</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga506348f89e2d32c08c1bac9f73afdb86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_STATIC_CLUTTER_SCRATCH</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gae2026854a04d4d0a5aa08bae373c3342</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_CFAR_OUT_DET_LIST</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga2c7e5b82f8cd90d0c88ff1d57ab8b1fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_OUT</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gacc1146cbaca54f6bf102fb75cd11a3f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_OUT_SIDE_INFO</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gab9491e75ae1b91183c4e1769df76df5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_2_AZIM_IDX</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gaf3ccf4afd5e1aa33ef88fcc84d023d5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_ELEVATION_ANGLE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gafaa15fc6b1c423a92a9f305b7a2582d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_AZIMUTH_STATIC_HEAT_MAP</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga39c0725e76ff7fce1aeec88eb76c82aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_AZIMUTH_FFT_MAGNITUDE</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gadde2b79670500cfaba5f0c0e2e7da8c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_SCRATCH_BUFFER</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>gabb08d6c6163079e00dfd5bc69f226c90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_PRE_START_CONFIG_BEFORE_PRE_START_COMMON_CONFIG</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga3fd5cf815de0648d09c5424bc00ed6ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_EINTERNAL</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga5ce1e64d19b80928f043d6436c01078e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DPC_OBJECTDETECTION_ENOTIMPL</name>
      <anchorfile>group___d_p_c___o_b_j_e_c_t_d_e_t_e_c_t_i_o_n___e_r_r_o_r___c_o_d_e.html</anchorfile>
      <anchor>ga6287c2038a0b61d0771d16a3072cdb5a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Object Detection Data-path Processing Chain (DPC)</title>
    <filename>index.html</filename>
    <docanchor file="index.html" title="Introduction">objdetintro</docanchor>
    <docanchor file="index.html" title="Data Path">objdethwa_datapath</docanchor>
    <docanchor file="index.html" title="Application-DPC Execution Flow">appdpcFlow</docanchor>
    <docanchor file="index.html" title="Real-Only Chirp Data">Handling</docanchor>
    <docanchor file="index.html" title="DPU reconfiguration related to data path processing within and across sub-frames">reconfig</docanchor>
    <docanchor file="index.html" title="Range Bias and Rx Channel Gain/Offset Measurement and Compensation">objdethwa_calibration</docanchor>
    <docanchor file="index.html">Figure_calibration</docanchor>
    <docanchor file="index.html" title="Measurement">measurement</docanchor>
    <docanchor file="index.html" title="Compensation">compensation</docanchor>
    <docanchor file="index.html" title="Data Path Design Notes">objdethwa_designNotes</docanchor>
    <docanchor file="index.html" title="Scaling">objdethwa_scaling</docanchor>
  </compound>
</tagfile>
