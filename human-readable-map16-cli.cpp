#include <iostream>
#include <exception>
#include <string>

#include "human_readable_map16.h"
#include "tile_error.h"
#include "header_error.h"
#include "filesystem_error.h"
#include "human_map16_exception.h"


void help() {
	std::cout << "Command-line program for converting from Lunar Magic's .map16 format to a human-readable text format" << std::endl << std::endl
		<< "[USAGE]: .\\human_readable_map16.exe --from-map16 <input map16 file path> <output directory path>" << std::endl
		<< "         .\\human_readable_map16.exe --to-map16 <input directory path> <output map16 file path>" << std::endl;
}

void from_map16(const fs::path input_map16, const fs::path output_directory) {
	try {
		HumanReadableMap16::from_map16::convert(input_map16, output_directory);
	}
	catch (HumanMap16Exception& e) {
		std::cerr << e.get_detailed_error_message() << std::endl;
		exit(1);
	}
	std::cout << input_map16 << " successfully converted to " << output_directory << std::endl;
}

void to_map16(const fs::path input_directory, const fs::path output_map16) {
	try {
		HumanReadableMap16::to_map16::convert(input_directory, output_map16);
	}
	catch (HumanMap16Exception& e) {
		std::cerr << e.get_detailed_error_message() << std::endl;
		exit(1);
	}
	std::cout << input_directory << " successfully converted to " << output_map16 << std::endl;
}

void try_drag_and_drop(const fs::path potential_path) {
	if (!fs::exists(potential_path)) {
		std::cerr << "Path " << potential_path << " does not exist" << std::endl;
		exit(1);
	}

	if (fs::is_directory(potential_path)) {
		to_map16(potential_path, potential_path.stem().string() + ".map16");
	} else {
		from_map16(potential_path, potential_path.stem());
	}

	exit(0);
}

void interactive_mode() {
	std::cout << "Press 0 for .map16 -> text conversion" << std::endl
		<< "Press 1 for text -> .map16 conversion" << std::endl;
	
	char input;
	while (std::cin >> input) {
		if (input != '0' && input != '1') {
			std::cout << "Please input 0 for .map16 -> text conversion or 1 for text -> .map16 conversion" << std::endl;
		} else {
			break;
		}
	}

	fs::path input_path, output_path;

	if (input == '0') {
		std::cout << "Path to input .map16 file: ";
		std::cin >> input_path;
		std::cout << "Path for output directory: ";
		std::cin >> output_path;
		from_map16(input_path, output_path);
	} else {
		std::cout << "Path to input directory: ";
		std::cin >> input_path;
		std::cout << "Path for output .map16 file: ";
		std::cin >> output_path;
		to_map16(input_path, output_path);
	}

	exit(0);
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		interactive_mode();
		exit(0);
	}

	if (argc <= 3) {
		if (argc == 2) {
			if (argv[1] == std::string("-h") || argv[1] == std::string("--help")) {
				help();
				exit(0);
			}
			else {
				try_drag_and_drop(argv[1]);
			}
		}
		std::cerr << "ERROR: Too few arguments provided, try -h or --help for program usage" << std::endl;
		exit(1);
	}

	if (argc > 4) {
		std::cerr << "ERROR: Too many arguments provided, try -h or --help for program usage" << std::endl;
		exit(1);
	}

	if (argv[1] == std::string("--from-map16")) {
		from_map16(argv[2], argv[3]);
		exit(0);
	}
	else if (argv[1] == std::string("--to-map16")) {
		to_map16(argv[2], argv[3]);
		exit(0);
	}
	else {
		std::cerr << "ERROR: Unknown argument, try -h or --help for program usage" << std::endl;
		exit(1);
	}
}
