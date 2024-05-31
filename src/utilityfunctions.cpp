#include "utilityfunctions.hpp"
using namespace std;

vector<string> cut_string(string str, string delim)
{

    int char_count = 0;
    int head = 0;
    vector<string> string_list;
    string new_string;
    int num_of_delim = count(str.begin(), str.end(), delim[0]);

    for (int i = 0; i < str.size(); i++)
    {
        string separator = str.substr(i, 1);
        char_count += 1;
        string a;
        if (separator == delim)
        {
            if (head == 0)
            {
                a = str.substr(head, char_count - 1);
                string_list.push_back(a);
                head = i;
                char_count = 0;
            }
            else
            {
                a = str.substr(head + 1, char_count - 1);
                string_list.push_back(a);
                head = i;
                char_count = 0;
            }
        }
        else if (num_of_delim == 0)
        {
            string_list.push_back(str);
            break;
        }

        else if (i == str.size() - 1)
        {
            a = str.substr(head + 1, char_count);
            string_list.push_back(a);
            head = i;
            char_count = 0;
        }
    }
    return string_list;
}

bool has_time_collision(string date1, string date2)
{
    vector<string> vec_date1 = cut_string(date1, ":");
    vector<string> vec_date2 = cut_string(date2, ":");
    string day1 = vec_date1[0];
    string day2 = vec_date2[0];
    string time1 = vec_date1[1];
    string time2 = vec_date2[1];
    if (day1 != day2)
    {
        return false;
    }
    int start1 = stoi(cut_string(time1, "-")[0]);
    int start2 = stoi(cut_string(time2, "-")[0]);
    int end1 = stoi(cut_string(time1, "-")[1]);
    int end2 = stoi(cut_string(time2, "-")[1]);
    if ((start1 >= start2 && start1 < end2) || (start2 >= start1 && start2 < end1))
    {
        return true;
    }
    else
    {
        return false;
    }
}





bool is_natural_number(const std::string &str)
{
    
    if (str.empty())
    {
        return false;
    }

    
    for (char ch : str)
    {
        if (!std::isdigit(ch))
        {
            return false;
        }
    }

    try
    {
       
        int num = std::stoi(str);

       
        if (num <= 0)
        {
            return false;
        }
    }
    catch (const std::invalid_argument &e)
    {
        throw BadRequestError();

    }
    catch (const std::out_of_range &e)
    {
       
        return false;
    }

    return true;
}
vector<string> read_csv(const char path[256])
{
    vector<string> lines;
    ifstream file_name;
    string new_line;
    file_name.open(path);
    while (getline(file_name, new_line))
    {
        lines.push_back(new_line);
    }
    file_name.close();
    return lines;
}