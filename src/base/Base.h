/**
*  @file Utils.h
*  @date 10/10/2020
*  @author MetalMario971
*/
#pragma once
#ifndef __UTILS_160500386112300559029158942552_H__
#define __UTILS_160500386112300559029158942552_H__

#include "../base/StringUtil.h"

namespace VG {
/**
*  @class Utility file for /base
*  @brief
*/
class Base {
public:
  static void initGlobals(std::shared_ptr<App> app, const string_t& logfile_dir, const std::vector<string_t>& args);
  static void updateGlobals();

  static std::shared_ptr<EngineConfig> config() { return _config; }

  static std::shared_ptr<Logger> logger() { return _logger; }

  static std::shared_ptr<App> app() { return _app; }
  static std::shared_ptr<Sequencer> sequencer() { return _sequencer; }

  static void checkErrorsDbg(bool ignore = false);
  static void checkErrorsRt(bool ignore = false);
  //**Going away

  static void debugBreak();
  static void checkMemory();
  static bool isDebug();
  static bool is64Bit();
  static bool isBigEndian();
  static string_t getOperatingSystemName();
  static std::string getCPPVersion();
  static void sleepThread(uint64_t milliseconds);

  static t_timeval getMicroSeconds();
  static t_timeval getMilliSeconds();

  static std::shared_ptr<Img32> loadImage(std::string path);
  static bool saveImage(std::string path, std::shared_ptr<Img32> spec);
  static int loadSound(std::string path, int& iChannels, int& iSampleRate, int16_t*& pData, int& nSamples, int& iDataLenBytes);
  static void print(char msg);
  static void print(const char* msg);
  static void print(const string_t& msg);
  static std::vector<string_t> argsToVectorOfString(int argc, char** argv, char delimiter = '=');

  template <typename Tx>
  static bool addIfDoesNotContain(std::vector<Tx>& vec, Tx& rhs) {
    for (size_t i = 0; i < vec.size(); ++i) {
      if (vec[i] == rhs) {
        return false;
      }
    }
    vec.push_back(rhs);
    return true;
  }
  template <typename Tx>
  static void incrementEnum(Tx& __in_ eValue, Tx iMaxValue) {
    eValue = (Tx)((int)eValue + 1);
    if ((int)eValue >= iMaxValue) {
      eValue = (Tx)0;
    }
  }
  static float fade(float t);
  static void checkErrors(bool log = true, bool bbreak = true);

  static bool checkArg(const std::vector<string_t>& args, const string_t& inkey, const string_t& invalue);
  static void parsearg(const string_t& arg, string_t& __out_ out_key, string_t& __out_ out_value);

private:
  static void createLogger(const string_t& logfile_dir, const std::vector<string_t>& args);
  static void loadConfig(const std::vector<std::string>& args);
  static void processArg(std::string key, std::string value);

  static std::shared_ptr<Logger> _logger;
  static std::shared_ptr<App> _app;
  static std::shared_ptr<Sequencer> _sequencer;
  static std::shared_ptr<EngineConfig> _config;
};

}  // namespace VG

#endif
