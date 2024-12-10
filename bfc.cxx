#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using namespace std::literals;

auto eval(const std::string& input, unsigned char* memptr) -> void
{
    auto loop { 0 };
    for (auto i { 0 }; input[i] != 0; ++i)
    {
        auto current { input[i] };

        switch (current)
        {
            case '>':
                memptr += 1;
                break;
            case '<':
                memptr -= 1;
                break;
            case '+':
                (*memptr)++;
                break;
            case '-':
                (*memptr)--;
                break;
            case '.':
                std::cout << *memptr << std::flush;
                break;
            case ',':
                std::cin >> *memptr;
                break;
            case '[':
                if (*memptr == 0)
                {
                    loop = 1;
                    while (loop > 0)
                    {
                        current = input[++i];
                        current == '[' ? loop++ 
                                       : current == ']' ? loop--
                                       : loop;
                    }
                }
				break;
			case ']':
                if (*memptr)
                {
                    loop = 1;
                    while (loop > 0)
                    {
                        current = input[--i];
                        current == '[' ? loop-- 
                                       : current == ']' ? loop++
                                       : loop;
                    }
                }
            default:
                break;
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    auto args = std::vector<std::string>{};
    std::ranges::copy_n(argv, argc, std::back_inserter(args));

    auto buffer = std::array<unsigned char, 2048>{ 0 };
    auto input = std::string{};

    if (args.size() < 3)
    {
        auto infile = std::ifstream(args.at(1), std::ios::in);

        if (infile.is_open())
        {
            std::ranges::copy_if(
                std::istream_iterator<char>(infile),
                std::istream_iterator<char>(),
                std::back_inserter(input),
                [chr=std::set{ '>', '<', '+', '-', '.', ',', '[', ']'}](const auto& x)
                { return chr.find(x) != chr.end(); }
            );
        }
        else
        {
            std::cerr << "Failed to open file " << args.at(1) << std::endl;
            return 1;
        }

        eval(input, buffer.begin());
    }
    else
    {
        std::cerr << "Wrong number of input files! Got " 
                  << args.size() - 1 
                  << ", expected 1. Aborting!" 
                  << std::endl;
        return 1;
    }

    std::cout << std::endl;

    return 0;
}
