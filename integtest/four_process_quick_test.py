import pytest

import dfmodules.data_file_checks as data_file_checks
import integrationtest.log_file_checks as log_file_checks

# Initialization
number_of_data_producers=2
expected_fragments_per_trigger_record=number_of_data_producers

# The next three variable declarations *must* be present as globals in the test
# file. They're read by the "fixtures" in conftest.py to determine how
# to run the config generation and nanorc

# The name of the python module for the config generation
confgen_name="minidaqapp.nanorc.mdapp_multiru_gen"
# The arguments to pass to the config generator, excluding the json
# output directory (the test framework handles that)
confgen_arguments=[ "-d", "./frames.bin", "-o", ".", "-s", "10", "-n", str(number_of_data_producers), "-b", "1000", "-a", "1000", "--host-ru", "localhost"]
# The commands to run in nanorc, as a list
nanorc_command_list="boot init conf start 101 wait 1 resume wait 20 pause wait 1 stop wait 2 scrap terminate".split()

import os
if "MDAPP_INTEGTEST_SWTPG" in os.environ:
    confgen_arguments.append("--enable-software-tpg")
    expected_fragments_per_trigger_record*=2
    print()
    print("*** Software TPG is enabled ***")
if "MDAPP_INTEGTEST_DQM" in os.environ:
    confgen_arguments.append("--enable-dqm")
    print()
    print("*** DQM is enabled ***")

# The tests themselves

def test_nanorc_success(run_nanorc):
    # Check that nanorc completed correctly
    assert run_nanorc.completed_process.returncode==0

def test_log_files(run_nanorc):
    # Check that there are no warnings or errors in the log files
    assert log_file_checks.logs_are_error_free(run_nanorc.log_files)

def test_data_file(run_nanorc):
    # Run some tests on the output data file
    assert len(run_nanorc.data_files)==1

    for idx in range(len(run_nanorc.data_files)):
        data_file=data_file_checks.DataFile(run_nanorc.data_files[idx])
        assert data_file_checks.sanity_check(data_file)
        assert data_file_checks.check_link_presence(data_file, n_links=expected_fragments_per_trigger_record)
        assert data_file_checks.check_fragment_sizes(data_file, min_frag_size=37200, max_frag_size=37200)
