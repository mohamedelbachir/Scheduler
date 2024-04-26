#pragma once
#ifndef PARSINGFILE_H_INCLUDED
#define PARSINGFILE_H_INCLUDED
void initParam(bool isOnSaving = false);
void initColorSetting(TiXmlElement *linkTo);
const char *initColorSetting();
void saveColorSetting();
void parseSetting(TiXmlElement *root);
#endif // PARSINGFILE_H_INCLUDED
