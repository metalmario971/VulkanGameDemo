#include "../base/Logger.h"
#include "../base/EngineConfig.h"

#include "../base/GLContext.h"
#include "../base/Gu.h"
#include "../base/Logger.h"
#include "../base/Exception.h"
#include "../base/HashMap.h"

namespace BR2 {
class EngineConfig_Internal {
public:
  HashMap<BaseProp*> _props;
};
EngineConfig::EngineConfig() {
  _pint = std::make_shared<EngineConfig_Internal>();
}
EngineConfig::~EngineConfig() {
}
void EngineConfig::int32Prop(string_t prop_key, std::vector<string_t>& tokens, int32_t& iind) {
  if (lcmp(tokens[0], prop_key.c_str(), 2)) {
    int32_t prop_value = TypeConv::strToInt(getCleanToken(tokens, iind));
    Int32Prop* p = new Int32Prop(prop_key, prop_value);
    //This may be an incorrect cast (not sure)
    _pint->_props.add(prop_key, (BaseProp*&)p);
  }
}
void EngineConfig::pkp(std::vector<string_t>& tokens) {
  int iind = 1;

  std::shared_ptr<EngineConfig> _pConfig = getThis<EngineConfig>();

  //Note: Don't put any OpenGL stuff in here.  It comes BEFORE we load the context.
  try {
    //TODO:
    //int32Prop(EngineConfig::MaxPointLights, tokens, iind);
    //int32Prop(EngineConfig::MaxDirLights, tokens, iind);

    if (lcmp(tokens[0], EngineConfig::c_MaxPointLights, 2)) {
      _pConfig->_iMaxPointLights = TypeConv::strToInt(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxDirLights, 2)) {
      _pConfig->_iMaxDirLights = TypeConv::strToInt(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxCubeShadowSamples, 2)) {
      _pConfig->_iMaxCubeShadowSamples = TypeConv::strToInt(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxFrustShadowSamples, 2)) {
      _pConfig->_iMaxFrustShadowSamples = TypeConv::strToInt(getCleanToken(tokens, iind));
    }

    else if (lcmp(tokens[0], EngineConfig::c_StartFullscreen, 2)) {
      _pConfig->_bStartFullscreen = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_DefaultScreenWidth, 2)) {
      _pConfig->_iDefaultScreenWidth = TypeConv::strToUint(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_DefaultScreenHeight, 2)) {
      _pConfig->_iDefaultScreenHeight = TypeConv::strToUint(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_EnableRuntimeErrorChecking, 2)) {
      _pConfig->_bEnableRuntimeErrorChecking = TypeConv::strToBool(getCleanToken(tokens, iind));
      if (Gu::isDebug()) {
        msg("Runtime error checking is turned on by default in debug build.");
        _pConfig->_bEnableRuntimeErrorChecking = true;
      }
      //*Always default true in debug mode
#ifdef _DEBUG
      _pConfig->_bEnableRuntimeErrorChecking = true;
#endif
    }
    else if (lcmp(tokens[0], EngineConfig::c_EnableFileLog, 2)) {
      _pConfig->_bEnableLogToFile = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_EnableConsoleLog, 2)) {
      _pConfig->_bEnableLogToFile = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_GameHostTimeoutMs, 2)) {
      _pConfig->_iGameHostTimeoutMs = TypeConv::strToInt(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_GameHostPort, 2)) {
      _pConfig->_iGameHostPort = TypeConv::strToInt(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_ForceAspectRatio, 2)) {
      _pConfig->_bForceAspectRatio = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_GpuDeviceName, 2)) {
      _pConfig->_strGpuDeviceName = getCleanToken(tokens, iind);
    }
    else if (lcmp(tokens[0], EngineConfig::c_DefaultFieldOfView, 2)) {
      _pConfig->_fDefaultFieldOfView = TypeConv::strToFloat(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_ColorSpace, 2)) {
      string_t csp = getCleanToken(tokens, iind);
      if (StringUtil::equalsi(csp, "SRGB")) {
        _pConfig->_eColorSpace = ColorSpace::e::SRGB;
      }
      else if (StringUtil::equalsi(csp, "Linear")) {
        _pConfig->_eColorSpace = ColorSpace::e::Linear;
      }
      else {
        msg(Stz "Unkown color space " + csp + " defaulting to linear.");
        _pConfig->_eColorSpace = ColorSpace::e::Linear;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_FontBitmapSize, 2)) {
      _pConfig->_iFontBitmapSize = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iFontBitmapSize < 5 || _pConfig->_iFontBitmapSize > 99999) {
        msg(tokens[0] + " invalid.  Defaulting to 1024");
        _pConfig->_iFontBitmapSize = 1024;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_FontBakedCharSize, 2)) {
      _pConfig->_iBakedCharSize = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iBakedCharSize < 5 || _pConfig->_iBakedCharSize > 999) {
        msg(tokens[0] + " was <5 ro > 999.  Defaulting to 64");
        _pConfig->_iBakedCharSize = 64;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_EnableObjectShadows, 2)) {
      _pConfig->_bEnableObjectShadows = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_EnableTerrainShadows, 2)) {
      _pConfig->_bEnableTerrainShadows = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_ShadowMapResolution, 2)) {
      _pConfig->_iShadowMapResolution = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (Alg::isPow2((int32_t)_pConfig->_iShadowMapResolution) == false) {
        msg(tokens[0] + " not a power of 2.  Defaulting to 256");
        _pConfig->_iShadowMapResolution = 256;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_ShadowMapMaxInfluences, 2)) {
      _pConfig->_iShadowMapMaxInfluences = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iShadowMapMaxInfluences > 32) {
        msg(tokens[0] + " >32.  Defaulting to 32");
        _pConfig->_iShadowMapMaxInfluences = 32;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxPointLightShadowDistance, 2)) {
      _pConfig->_fMaxPointLightShadowDistance = TypeConv::strToFloat(getCleanToken(tokens, iind));
      if (_pConfig->_fMaxPointLightShadowDistance > 2000.0f) {
        msg(tokens[0] + " >2000.0f.  Defaulting to 2000.0f");
        _pConfig->_fMaxPointLightShadowDistance = 2000.0f;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_EnableMSAA, 2)) {
      _pConfig->_bEnableMsaa = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_MSAASamples, 2)) {
      _pConfig->_iMsaaSamples = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iMsaaSamples < 0) {
        msg(Stz "MSAA Samples" + _pConfig->_iMsaaSamples + " invalid.");
        _pConfig->_iMsaaSamples = 2;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_NumTextQuads, 2)) {
      _pConfig->_iNumTextQuads = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iNumTextQuads <= 256) {
        msg(tokens[0] + " Too small, set to 256.");
        _pConfig->_iNumTextQuads = 256;
      }
      if (_pConfig->_iNumTextQuads > 16384) {
        msg(tokens[0] + " Too large.  Set to 16384.");
        _pConfig->_iNumTextQuads = 16384;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_BreakOnSDLError, 2)) {
      _pConfig->_bBreakOnSDLError = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_BreakOnGraphicsError, 2)) {
      //Break if there is an error in the graphics system.
      _pConfig->_bBreakOnGraphicsError = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_ShowConsole, 2)) {
      _pConfig->_bShowConsole = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_ModelThumbSize, 2)) {
      _pConfig->_iModelThumbSize = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iModelThumbSize < 1) {
        _pConfig->_iModelThumbSize = 1;
      }
      if (_pConfig->_iModelThumbSize > 4192) {
        _pConfig->_iModelThumbSize = 4192;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxHardwareIncomingBufferSizeBytes, 2)) {
      _pConfig->_iMaxHardwareIncomingBufferSizeBytes = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iMaxHardwareIncomingBufferSizeBytes < 1) {
        _pConfig->_iMaxHardwareIncomingBufferSizeBytes = 1;
      }
      if (_pConfig->_iMaxHardwareIncomingBufferSizeBytes > 9999999) {
        _pConfig->_iMaxHardwareIncomingBufferSizeBytes = 9999999;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxHardwareOutgoingBufferSizeBytes, 2)) {
      _pConfig->_iMaxHardwareOutgoingBufferSizeBytes = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iMaxHardwareOutgoingBufferSizeBytes < 1) {
        _pConfig->_iMaxHardwareOutgoingBufferSizeBytes = 1;
      }
      if (_pConfig->_iMaxHardwareOutgoingBufferSizeBytes > 9999999) {
        _pConfig->_iMaxHardwareOutgoingBufferSizeBytes = 9999999;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxSoftwareIncomingBufferSizeBytes, 2)) {
      _pConfig->_iMaxSoftwareIncomingBufferSizeBytes = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iMaxSoftwareIncomingBufferSizeBytes < 1) {
        _pConfig->_iMaxSoftwareIncomingBufferSizeBytes = 1;
      }
      if (_pConfig->_iMaxSoftwareIncomingBufferSizeBytes > 9999999) {
        _pConfig->_iMaxSoftwareIncomingBufferSizeBytes = 9999999;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_MaxSoftwareOutgoingBufferSizeBytes, 2)) {
      _pConfig->_iMaxSoftwareOutgoingBufferSizeBytes = TypeConv::strToInt(getCleanToken(tokens, iind));
      if (_pConfig->_iMaxSoftwareOutgoingBufferSizeBytes < 1) {
        _pConfig->_iMaxSoftwareOutgoingBufferSizeBytes = 1;
      }
      if (_pConfig->_iMaxSoftwareOutgoingBufferSizeBytes > 9999999) {
        _pConfig->_iMaxSoftwareOutgoingBufferSizeBytes = 9999999;
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_RenderSystem, 2)) {
      string_t tok = getCleanToken(tokens, iind);

      _pConfig->_eRenderSystem = RenderSystem::OpenGL;
      if (StringUtil::equalsi(tok, "Vulkan")) {
        _pConfig->_eRenderSystem = RenderSystem::Vulkan;
      }
      else if (StringUtil::equalsi(tok, "OpenGL")) {
        _pConfig->_eRenderSystem = RenderSystem::OpenGL;
      }
      else {
        msg(Stz "Render system '" + tok + "' is invalid.");
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_WindowedAspectRatio, 2)) {
      string_t st = getCleanToken(tokens, iind);
      double num, dem;
      std::vector<std::string> vs = StringUtil::split(st, '/');
      if (vs.size() == 2) {
        num = TypeConv::strToDouble(vs[0]);
        dem = TypeConv::strToDouble(vs[1]);
        _pConfig->_windowedAspectRatio = num / dem;
      }
      else {
        msg(Stz "Failed to read '" + tokens[0] + "', must be in the form uint/uint, and width/height, ex: 1920/1080.");
        Gu::debugBreak();
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_GamePad, 2)) {
      string_t tok = getCleanToken(tokens, iind);
      if (StringUtil::equalsi(tok, "KeyboardAndMouse")) {
        _pConfig->_eGamePadType = GamePadType::KeyboardAndMouse;
      }
      else {
        msg(Stz "Invalid token for: '" + tokens[0] + "' got: '" + tok + "'.");
        Gu::debugBreak();
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_OpenGLProfile, 2)) {
      string_t tok = getCleanToken(tokens, iind);
      if (StringUtil::equalsi(tok, "Compatibility")) {
        _pConfig->_eOpenGLProfile = OpenGLProfile::Compatibility;
      }
      else if (StringUtil::equalsi(tok, "Core")) {
        _pConfig->_eOpenGLProfile = OpenGLProfile::Core;
      }
      else if (StringUtil::equalsi(tok, "ES")) {
        //Technically, this should be automatic if we are sure we're on the phone.
        _pConfig->_eOpenGLProfile = OpenGLProfile::ES;
      }
      else {
        msg(Stz "Invalid token for: '" + tokens[0] + "' got: '" + tok + "'.");
        Gu::debugBreak();
      }
    }
    else if (lcmp(tokens[0], EngineConfig::c_Linux_ReadableBacktrace, 2)) {
      _pConfig->_bLinux_ReadableBacktrace = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_GraphicsErrorLogging_High, 2)) {
      _pConfig->_bGraphicsErrorLogging_High = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_GraphicsErrorLogging_Medium, 2)) {
      _pConfig->_bGraphicsErrorLogging_Medium = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_GraphicsErrorLogging_Low, 2)) {
      _pConfig->_bGraphicsErrorLogging_Low = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else if (lcmp(tokens[0], EngineConfig::c_GraphicsErrorLogging_Info, 2)) {
      _pConfig->_bGraphicsErrorLogging_Info = TypeConv::strToBool(getCleanToken(tokens, iind));
    }
    else {
      msg(Stz " Unrecognized engine config token '" + tokens[0] + "'");
      Gu::debugBreak();
    }
  }
  catch (Exception* ex) {
    BRLogErrorEx("Failed to parse '" + (tokens.size() > 0 ? tokens[0] : "<invalid>") + "'in the engine configuration file ", ex);
  }
}
void EngineConfig::preLoad() {
}
void EngineConfig::postLoad(bool success) {
}

}  // namespace BR2
