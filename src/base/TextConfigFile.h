/**
*  @file TextConfigFile.h
*  @date August 4, 2019
*  @author MetalMario971
*/
#pragma once
#ifndef __PARSABLEFILE_156494899751183385_H__
#define __PARSABLEFILE_156494899751183385_H__

#include "../base/TextDataFile.h"

namespace BR2 {
/**
*  @class TextConfigFile
*  @brief Any file that can be processed as a list of string name-value tokens.
*/
class TextConfigFile : public TextDataFile {
public:
  TextConfigFile() {}
  TextConfigFile(const string_t& loc);
  virtual ~TextConfigFile() override;

  bool lcmp(const string_t& tok0, const char* ch, int32_t validateArgCount = -1);
  bool lcmp(const string_t& tok0, const std::string& ch, int32_t validateArgCount = -1);
  void parseErr(const string_t& str, bool bDebugBreak = false, bool bFatal = false);
  string_t getCleanToken(std::vector<string_t>& tokens, int& iind);
  string_t getCleanTokenInd(std::vector<string_t>& tokens, int iind);

protected:
  int32_t _iCurrentParseLine = 1;    //crurent parse line.
  std::vector<string_t> _vecTokens;

  //Send in a list of parameters [key] [val] [val] [val]
  virtual void pkp(std::vector<string_t>& tokens) = 0;// Must be implemented

  //Utils.
  virtual bool validateArguments(const string_t& token, int32_t count = -1);    // throw exception
  void displayError(const string_t& errMsg, bool bThrow = true);
  bool cmp(const string_t& tok0, const char* ch, int32_t validateArgCount);
};

}//ns Game



#endif
