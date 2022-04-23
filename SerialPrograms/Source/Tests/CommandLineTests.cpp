/*  Command Line Tests
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */


#include "CommandLineTests.h"
#include "CommonFramework/GlobalSettingsPanel.h"
#include "PokemonLA_Tests.h"
#include "TestMap.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <list>
#include <functional>
using std::cout;
using std::cerr;
using std::endl;

namespace PokemonAutomation{


// CommandLineTests/                    <- root test folder
// - PokemonLA/                         <- test space to organize tests
//   - BattleMenuDetector/              <- test object, name of the class/function/file to test
//     - IngoBattleDayTime-True.png     <- test file

namespace{

const char* TEST_FOLDER_NAME = "CommandLineTests/";

#define RETURN_IF_NOT_ZERO(statement) \
    do { \
        int _ret = (statement); \
        if (_ret != 0){ \
            return _ret; \
        } \
    } while (0)

#define RETURN_IF_TEST_FAILED(test_func, file_path, num_passed) \
    do { \
        int _ret = test_func(file_path); \
        if (_ret > 0) {\
            cout << "===========================================" << endl; \
            cout << "Test: " << (file_path) << " failed." << endl; \
            return _ret; \
        } else if (_ret == 0){ \
            num_passed++; \
        } \
    } while (0)


int run_test_obj_dir(TestFunction test_func, const QString& directory_path, size_t& num_passed){
    QDirIterator file_iter(directory_path, QDir::Filter::Files, QDirIterator::IteratorFlag::Subdirectories);

    bool first_test_file = true;
    while (file_iter.hasNext()){
        const std::string file_path = file_iter.next().toStdString();
        if (first_test_file == false){
            cout << "-------------------------------------------" << endl;
        }
        first_test_file = false;

        // Call the function to do the actual test:
        RETURN_IF_TEST_FAILED(test_func, file_path, num_passed);
    }

    return 0;
}

int run_test_obj(const std::string& test_space, const QFileInfo& obj_info, size_t& num_passed){
    // test_name e.g.: "BattleMenuDetector"
    const std::string test_name = obj_info.fileName().toStdString();
    if (test_name == "." || test_name == ".."){
        return 0;
    }

    const TestFunction test_func = find_test_function(test_space, test_name);
    if (test_func == nullptr){
        return 2;
    }

    cout << "===========================================" << endl;
    cout << "Testing " << test_name << ":" << endl;

    // Recursively get test filenames, like:
    // ./CommandLineTests/PokemonLA/BattleMenuDetector/IngoBattleMenuDayTime-True.png
    return run_test_obj_dir(test_func, obj_info.filePath(), num_passed);
}


int run_test_space(const QFileInfo& space_info, size_t& num_passed){
    // sub_dir e.g.: ./CommandLineTests/PokemonLA/
    QDir sub_dir(space_info.filePath());
    if (!sub_dir.exists()){
        cerr << "Error: cannot access " << space_info.filePath().toStdString() << endl;
        return 1;
    }

    sub_dir.setFilter(QDir::Filter::Dirs);
    // test_space e.g "PokemonLA". It's like the namespace.
    const std::string test_space = space_info.fileName().toStdString();
    if (test_space == "." || test_space == ".."){
        return 0;
    }

    // Look for sub-sub-folders as test object names, e.g.
    // ./CommandLineTests/PokemonLA/BattleMenuDetector/
    const QFileInfoList obj_list = sub_dir.entryInfoList();
    for(const QFileInfo& obj_info : obj_list){
        RETURN_IF_NOT_ZERO(run_test_obj(test_space, obj_info, num_passed));
    }

    return 0;
}




} // end of anonymous namespace



int run_command_line_tests(){
    QDir test_root_dir(TEST_FOLDER_NAME);
    if (!test_root_dir.exists()){
        cerr << "Error: command line test folder ./" << TEST_FOLDER_NAME << " does not exist." << endl;
        return 1;
    }

    QFileInfo test_root_info(TEST_FOLDER_NAME);

    size_t num_passed = 0;

    const auto& selected_test_list = GlobalSettings::instance().COMMAND_LINE_TEST_LIST;

    // Run all tests
    if (selected_test_list.size() == 0){
        // Look for sub-folders, e.g.
        // ./CommandLineTests/PokemonLA/
        // ./CommandLineTests/PokemonSwSh/
        test_root_dir.setFilter(QDir::Filter::Dirs);
        const QFileInfoList sub_dir_list = test_root_dir.entryInfoList();
        for(const QFileInfo& sub_dir_info : sub_dir_list){
            RETURN_IF_NOT_ZERO(run_test_space(sub_dir_info, num_passed));
        }
    } else{
        // Only run on selected tests
        for(const std::string& test_path : selected_test_list){
            const std::string full_path = TEST_FOLDER_NAME + test_path;
            const QString full_path_cleaned = QDir::cleanPath(QString::fromStdString(full_path));

            if (full_path_cleaned.size() == 0){
                cerr << "Error: empty path found in TEST_LIST" << endl;
                return 1;
            }

            QFileInfo selected_path_info(full_path_cleaned);

            if (selected_path_info.exists() == false){
                cerr << "Error: path " << full_path << " in TEST_LIST does not exist." << endl;
                return 1;
            }

            std::list<QString> path_components;
            {
                QString path = full_path_cleaned;
                QFileInfo cur_info(path);
                while(cur_info != test_root_info) {
                    path_components.push_front(cur_info.fileName());
                    // Go upper one level of folder:
                    path = cur_info.path();
                    cur_info = QFileInfo(path);
                }
            }
            // If full_path is "PokemonLA/DialogueEllipseDetector/macOS_bright/WendyNight-True.png", then
            // path_components contains:
            // - PokemonLA
            // - DialogueEllipseDetector
            // - macOS_bright
            // - WendyNight-True.png
            if (path_components.size() == 0){
                cerr << "Error: cannot parse " << full_path << ". Empty path in TEST_LIST?" << endl;
                return 1;
            }

            QDir cur_dir(TEST_FOLDER_NAME);

            auto it = path_components.begin();
            std::string test_space = it->toStdString();
            QFileInfo test_space_info(cur_dir.filePath(*it));
            cur_dir = QDir(test_space_info.filePath());
            if (path_components.size() == 1){
                RETURN_IF_NOT_ZERO(run_test_space(test_space_info, num_passed));
                continue;
            }

            it++;
            std::string test_name = it->toStdString();
            QFileInfo test_obj_info(cur_dir.filePath(*it));
            if (path_components.size() == 2){
                RETURN_IF_NOT_ZERO(run_test_obj(test_space, test_obj_info, num_passed));
                continue;
            }

            const auto test_func = find_test_function(test_space, test_name);
            if (test_func == nullptr){
                return 2;
            }

            cout << "===========================================" << endl;
            if (selected_path_info.isFile()){
                // Call the function to do the actual test:
                RETURN_IF_TEST_FAILED(test_func, full_path_cleaned.toStdString(), num_passed);
            } else{
                // selected_path_info is a directory, go through each file recursively in the directory
                RETURN_IF_NOT_ZERO(run_test_obj_dir(test_func, full_path_cleaned, num_passed));
            }
        } // end selected_test_list
    }

    cout << "===========================================" << endl;
    cout << num_passed << " test" << (num_passed > 1 ? "s" : "") << " passed" << std::endl;
    return 0;
}


}