//
// Created by dram on 14.05.18.
//

#ifndef FILEEXCHANGER_CONSTVARIABLES_H
#define FILEEXCHANGER_CONSTVARIABLES_H

#include <string>

//in client
static const std::string clientSettingsFile = "clientSettings.db";

static const std::string clientSharedFolderKey = "shared-folder";
static const std::string brokerIpKey = "broker-ip";

static const std::string brokerSettingsFile = "brokerSettings.db";

static const std::string brokerSharedDirectory = "broker_shared/";

static const int fileNameMaxLength = 40;
#endif //FILEEXCHANGER_CONSTVARIABLES_H
