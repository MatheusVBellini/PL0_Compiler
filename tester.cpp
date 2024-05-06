#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <tuple>

using namespace std;

bool ends_with(const std::string &str, const std::string &suffix)
{
    if (str.length() < suffix.length())
    {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "\033[1;31mError:\033[0m Usage: " << argv[0] << " <folder>" << endl;
        return EXIT_FAILURE;
    }

    string folder_name = argv[1];
    string test_folder = folder_name + "/tests/";
    if (!filesystem::is_directory(test_folder))
    {
        cerr << "\033[1;31mError:\033[0m " << test_folder << " does not exist or is not a directory." << endl;
        return EXIT_FAILURE;
    }

    string compile_command = "make";
    int compile_result = system(compile_command.c_str());
    if (compile_result != 0)
    {
        cerr << "\033[1;31mError:\033[0m Failed to compile program" << endl;
        return EXIT_FAILURE;
    }

    string program = "./lexical_analyser";

    vector<tuple<string, bool, double>> results;

    for (const auto &entry : filesystem::directory_iterator(test_folder))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        string input_file = entry.path().string();
        if (!ends_with(input_file, ".in"))
        {
            continue;
        }

        string output_file = input_file.substr(0, input_file.size() - 3) + ".out";

        ifstream ifs(input_file);
        if (!ifs)
        {
            cerr << "\033[1;31mError:\033[0m Could not open input file " << input_file << endl;
            results.emplace_back(input_file, false, 0);
            continue;
        }

        ostringstream oss;
        oss << program << " " << input_file << " > output.txt 2>&1";
        cout << oss.str() << endl;
        string command = oss.str();

        auto start_time = chrono::high_resolution_clock::now();
        int result = system(command.c_str());
        auto end_time = chrono::high_resolution_clock::now();

        double elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;

        if (result != 0)
        {
            cerr << "\033[1;31mError:\033[0m Failed to run program " << program << endl;
            results.emplace_back(input_file, false, elapsed_time);
            continue;
        }

        ifstream ifs_output(output_file);
        if (!ifs_output)
        {
            cerr << "\033[1;31mError:\033[0m Could not open output file " << output_file << endl;
            results.emplace_back(input_file, false, elapsed_time);
            continue;
        }

        string expected_output;
        getline(ifs_output, expected_output, '\0');

        ostringstream oss_actual_output;
        ifstream ifs_actual_output("output.txt");
        oss_actual_output << ifs_actual_output.rdbuf();
        string actual_output = oss_actual_output.str();

        if (actual_output == expected_output)
        {
            results.emplace_back(input_file, true, elapsed_time);
        }
        else
        {
            cout << "\033[1;31mTest " << input_file << " failed\033[0m" << endl;

            // colorize diff
            string diff_output;
            istringstream iss_expected_output(expected_output);
            istringstream iss_actual_output(actual_output);
            string line_expected_output;
            string line_actual_output;
            while (getline(iss_expected_output, line_expected_output) && getline(iss_actual_output, line_actual_output))
            {
                if (line_expected_output == line_actual_output)
                {
                    diff_output += line_expected_output + "\n";
                }
                else
                {
                    diff_output += "\033[1;32m" + line_expected_output + "\033[0m\n"; // paint the expected output in green
                    diff_output += "\033[1;31m" + line_actual_output + "\033[0m\n"; // paint the actual output in red
                }
            }

            cout << diff_output << endl;

            results.emplace_back(input_file, false, elapsed_time);
        }
    }

    cout << endl;
    cout << "CPPTester 1.0.0 by Heitor Tanoue" << endl << endl;
    cout << "\033[1m[" << test_folder << "]\033[0m" << endl;
    int passed = 0;
    int failed = 0;
    for (const auto &result : results)
    {
        cout << "\033[1;30m|---\033[0m " << get<0>(result) << ": ";
        if (get<1>(result))
        {
            cout << "\033[1;32mpassed ✔ \033[0;30m[" << get<2>(result) << "s]" << endl;
            passed++;
        }
        else
        {
            cout << "\033[1;31mfailed ✘ \033[0;30m[" << get<2>(result) << "s]" << endl;
            failed++;
        }
    }
    cout << endl;
    cout << "\033[1;32m[🟢 Passed: " << passed << "]\033[0m" << endl;
    cout << "\033[1;31m[🔴 Failed: " << failed << "]\033[0m" << endl;

    remove("output.txt");
    remove(program.c_str());

    return (failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
