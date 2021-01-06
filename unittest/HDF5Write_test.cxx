/**
 * @file HDF5Write_test.cxx Application that tests and demonstrates
 * the write functionality of the HDF5DataStore class.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

//#include "dfmodules/DataStore.hpp"
//#include "dfmodules/hdf5datastore/Nljs.hpp"
//#include "dfmodules/hdf5datastore/Structs.hpp"
#include "../plugins/HDF5DataStore.hpp"

#include "ers/ers.h"

#define BOOST_TEST_MODULE HDF5Write_test // NOLINT

#include <boost/test/unit_test.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <vector>

using namespace dunedaq::dfmodules;

std::vector<std::string>
getFilesMatchingPattern(const std::string& path, const std::string& pattern)
{
  std::regex regexSearchPattern(pattern);
  std::vector<std::string> fileList;
  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    if (std::regex_match(entry.path().filename().string(), regexSearchPattern)) {
      fileList.push_back(entry.path());
    }
  }
  return fileList;
}

std::vector<std::string>
deleteFilesMatchingPattern(const std::string& path, const std::string& pattern)
{
  std::regex regexSearchPattern(pattern);
  std::vector<std::string> fileList;
  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    if (std::regex_match(entry.path().filename().string(), regexSearchPattern)) {
      if (std::filesystem::remove(entry.path())) {
        fileList.push_back(entry.path());
      }
    }
  }
  return fileList;
}

BOOST_AUTO_TEST_SUITE(HDF5Write_test)

/*
BOOST_AUTO_TEST_CASE(WriteFragmentFiles)
{
  std::string filePath(std::filesystem::temp_directory_path());
  std::string filePrefix = "demo" + std::to_string(getpid());
  const int EVENT_COUNT = 7;
  const int GEOLOC_COUNT = 4;
  const int DUMMYDATA_SIZE = 100;

  // delete any pre-existing files so that we start with a clean slate
  std::string deletePattern = filePrefix + ".*.hdf5";
  deleteFilesMatchingPattern(filePath, deletePattern);

  // create the DataStore
  nlohmann::json conf ;
  conf["name"] = "tempWriter" ;
  conf["filename_prefix"] = filePrefix ; 
  conf["directory_path"] = filePath ; 
  conf["mode"] = "one-fragment-per-file" ;
  
  std::unique_ptr<HDF5DataStore> dsPtr(new HDF5DataStore(conf));

  // write several events, each with several fragments
  char dummyData[DUMMYDATA_SIZE];
  for (int eventID = 1; eventID <= EVENT_COUNT; ++eventID) {
    for (int geoLoc = 0; geoLoc < GEOLOC_COUNT; ++geoLoc) {
      StorageKey key(eventID, StorageKey::INVALID_DETECTORID, geoLoc);
      KeyedDataBlock dataBlock(key);
      dataBlock.unowned_data_start = static_cast<void*>(&dummyData[0]);
      dataBlock.data_size = DUMMYDATA_SIZE;
      dsPtr->write(dataBlock);
    }
  }
  dsPtr.reset(); // explicit destruction

  // check that the expected number of files was created
  std::string searchPattern = filePrefix + ".*event.*geoID.*.hdf5";
  std::vector<std::string> fileList = getFilesMatchingPattern(filePath, searchPattern);
  BOOST_REQUIRE_EQUAL(fileList.size(), (EVENT_COUNT * GEOLOC_COUNT));

  // clean up the files that were created
  fileList = deleteFilesMatchingPattern(filePath, deletePattern);
  BOOST_REQUIRE_EQUAL(fileList.size(), (EVENT_COUNT * GEOLOC_COUNT));
}

BOOST_AUTO_TEST_CASE(WriteEventFiles)
{
  std::string filePath(std::filesystem::temp_directory_path());
  std::string filePrefix = "demo" + std::to_string(getpid());
  const int EVENT_COUNT = 7;
  const int GEOLOC_COUNT = 4;
  const int DUMMYDATA_SIZE = 100;

  // delete any pre-existing files so that we start with a clean slate
  std::string deletePattern = filePrefix + ".*.hdf5";
  deleteFilesMatchingPattern(filePath, deletePattern);

  // create the DataStore
  nlohmann::json conf ;
  conf["name"] = "tempWriter" ;
  conf["filename_prefix"] = filePrefix ; 
  conf["directory_path"] = filePath ; 
  conf["mode"] = "one-event-per-file" ;
  std::unique_ptr<HDF5DataStore> dsPtr(new HDF5DataStore(conf));

  // write several events, each with several fragments
  char dummyData[DUMMYDATA_SIZE];
  for (int eventID = 1; eventID <= EVENT_COUNT; ++eventID) {
    for (int geoLoc = 0; geoLoc < GEOLOC_COUNT; ++geoLoc) {
      StorageKey key(eventID, StorageKey::INVALID_DETECTORID, geoLoc);
      KeyedDataBlock dataBlock(key);
      dataBlock.unowned_data_start = static_cast<void*>(&dummyData[0]);
      dataBlock.data_size = DUMMYDATA_SIZE;
      dsPtr->write(dataBlock);
    }
  }
  dsPtr.reset(); // explicit destruction

  // check that the expected number of files was created
  std::string searchPattern = filePrefix + ".*event.*.hdf5";
  std::vector<std::string> fileList = getFilesMatchingPattern(filePath, searchPattern);
  BOOST_REQUIRE_EQUAL(fileList.size(), EVENT_COUNT);

  // clean up the files that were created
  fileList = deleteFilesMatchingPattern(filePath, deletePattern);
  BOOST_REQUIRE_EQUAL(fileList.size(), EVENT_COUNT);
}
*/

BOOST_AUTO_TEST_CASE(WriteOneFile)
{
  std::string filePath(std::filesystem::temp_directory_path());
  std::string filePrefix = "demo" + std::to_string(getpid());

  const int DUMMYDATA_SIZE = 7;  
  const int RUN_NUMBER = 52;
  const int TRIGGER_COUNT = 5;
  const std::string DETECTOR = "FELIX";
  const int APA_COUNT = 3;
  const int LINK_COUNT = 1;
 
  // delete any pre-existing files so that we start with a clean slate
  std::string deletePattern = filePrefix + ".*.hdf5";
  deleteFilesMatchingPattern(filePath, deletePattern);

  std::cout << "Current path is " << std::filesystem::current_path() << '\n';

  // create the DataStore
  nlohmann::json conf ;
  conf["name"] = "tempWriter" ;
  conf["directory_path"] = filePath ; 
  conf["mode"] = "all-per-file" ;
  nlohmann::json subconf ;
  subconf["overall_prefix"] = filePrefix ; 
  conf["filename_parameters"] = subconf ;
  std::unique_ptr<HDF5DataStore> dsPtr(new HDF5DataStore(conf));
#if 0
  hdf5datastore::ConfParams config_params;
  config_params.name = "tempWriter";
  config_params.mode = "all-per-file";
  config_params.max_file_size_bytes = 10000000;  // much larger than what we expect, so no second file;
  config_params.directory_path = filePath;
  config_params.filename_parameters.overall_prefix = filePrefix;
  hdf5datastore::data_t hdf5ds_json;
  hdf5datastore::to_json(hdf5ds_json, config_params);
  std::unique_ptr<DataStore> dsPtr;
  dsPtr = makeDataStore(hdf5ds_json);
#endif

  // write several events, each with several fragments
  char dummyData[DUMMYDATA_SIZE];
  for (int triggerNumber = 1; triggerNumber <= TRIGGER_COUNT; ++triggerNumber) {
    for (int apaNumber = 1; apaNumber <= APA_COUNT; ++apaNumber) {
      for (int linkNumber = 1; linkNumber <= LINK_COUNT; ++linkNumber) {
        StorageKey key(RUN_NUMBER, triggerNumber, DETECTOR, apaNumber, linkNumber);
        KeyedDataBlock dataBlock(key);
        dataBlock.unowned_data_start = static_cast<void*>(&dummyData[0]);
        dataBlock.data_size = DUMMYDATA_SIZE;
        dsPtr->write(dataBlock);
      } //link number
    } // apa number
  } // trigger number
  dsPtr.reset(); // explicit destruction

  // check that the expected number of files was created
  std::string searchPattern = filePrefix + ".*.hdf5";
  std::vector<std::string> fileList = getFilesMatchingPattern(filePath, searchPattern);
  BOOST_REQUIRE_EQUAL(fileList.size(), 1);

  // clean up the files that were created
  //fileList = deleteFilesMatchingPattern(filePath, deletePattern);
  //BOOST_REQUIRE_EQUAL(fileList.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
