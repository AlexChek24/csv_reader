#include "CSV_reader.h"

CSV_reader::CSV_reader(std::string path)
{
    file_.open(path);
}

void CSV_reader::Read()
{
    std::string str;
    int count_line { 0 };
    std::regex chek_columns("[\s*||A-Za-z]*");
    std::regex chek_rows("[1-9][0-9]*");
    std::stringstream ss;
    while (std::getline(file_, str))
    {
        std::stringstream ss(str);
        std::string tmp;

        if (count_line == 0) {
            while (std::getline(ss, tmp, ',')) {
                if(!std::regex_match(tmp.c_str(),chek_columns)){
                    throw std::runtime_error("Неверный формат заголовков");
                }

                columns_.push_back(tmp);
            }
            count_line++;
        }
        else {
            while (std::getline(ss, tmp, ','))
                any_cell_.push_back(tmp);
        }
        std::getline(file_, str, ',');
        if (!file_.eof()) {
            if(!std::regex_match(str.c_str(),chek_rows)){
                throw std::runtime_error ("Неверный формат строк");
            }
            rows_.push_back(str);
        }
    }

    auto IsNumber = [](const std::string& s) {
        return all_of(s.cbegin(), s.cend(), [](const char& c) {
            return isdigit(c);
        });
    };
    int k = 0;
    for (int i = 0; i < rows_.size(); i++)
    {
        for (int j = 1; j < columns_.size(); j++)
        {
            std::string tmp = (columns_[j] + rows_[i]);
            bool flag = IsNumber(any_cell_[k]);
            shd_matrix_.insert(std::make_pair(tmp, std::make_pair(flag, any_cell_[k])));
            k++;
        }

    }
}

void CSV_reader::Parse()
{
    auto erase_eq_or_space = [](char c)
    {
        if (c == ' ' || c == '=')
            return true;
        else
            return false;
    };
    auto calculate_expression = [](std::string& src_string, const std::string arg1, const std::string arg2)
    {
        if (src_string.find('+') != std::string::npos)
        {
            src_string = std::to_string(std::stoi(arg1) + std::stoi(arg2));
        }
        if (src_string.find('-') != std::string::npos)
        {
            src_string = std::to_string(std::stoi(arg1) - std::stoi(arg2));
        }
        if (src_string.find('/') != std::string::npos)
        {
            if(std::stoi(arg2)==0)
            {
                throw std::runtime_error("Деление на ноль");
            }
            else {
                src_string = std::to_string(std::stoi(arg1) / std::stoi(arg2));
            }
        }
        if (src_string.find('*') != std::string::npos)
        {
            src_string = std::to_string(std::stoi(arg1) * std::stoi(arg2));
        }
    };

    uint32_t counter_flags{ 0 };
    while (counter_flags < shd_matrix_.size()) {
        counter_flags = 0;
        for (auto& x : shd_matrix_)
        {
            if (false == x.second.first) {
                x.second.second.erase(std::remove_if(x.second.second.begin(), x.second.second.end(), erase_eq_or_space), x.second.second.end());
                std::regex regular_op("([\-*+/])");
                std::vector <std::string> Args(std::sregex_token_iterator(x.second.second.begin(), x.second.second.end(), regular_op, -1)
                        , std::sregex_token_iterator());
                auto Arg1 = shd_matrix_.find(Args[0]);
                auto Arg2 = shd_matrix_.find(Args[1]);

                if (Arg1 == shd_matrix_.end() || Arg2 == shd_matrix_.end())
                {
                    throw std::runtime_error("Не верная запись в ячейке "+x.first+'\n');
                }
                if (!Arg1->second.first || !Arg2->second.first)
                {
                    continue;
                }
                else {
                    calculate_expression(x.second.second, Arg1->second.second, Arg2->second.second);
                }
                x.second.first = true;
                counter_flags++;
            }
            else
            {
                counter_flags++;
            }

        }
    }
}

void CSV_reader::Write() const
{
    for (int j{ 0 }; j < columns_.size(); j++)//write columns_
    {
        if (j == columns_.size() - 1)
        {
            std::cout << columns_[j] << '\n';
        }
        else
        {
            std::cout << columns_[j] << ',';
        }
    }
    int k{ 0 };

    for (int i{ 0 }; i < rows_.size(); i++)
    {
        for (int j{ 1 }; j < columns_.size(); j++)
        {
            std::string tmp = (columns_[j] + rows_[i]);
            auto any_cell_ = shd_matrix_.find(tmp);
            if (j == 1)
            {
                std::cout << rows_[i] << ",";
                std::cout << any_cell_->second.second << ',';
            }
            else
            {
                if (j == columns_.size() - 1)
                {
                    std::cout << any_cell_->second.second << '\n';
                }
                else
                {
                    std::cout << any_cell_->second.second << ',';
                }
            }
        }

    }
}
