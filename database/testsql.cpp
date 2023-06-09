#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    stringstream ss;
    
    
    int num_rows; // 生成 1000000 条数据，可以根据需要进行修改
    cin>>num_rows;
    for(int i = 1; i <= num_rows; i++) {
        cout << "INSERT INTO activity_info (activity_id, activity_name, user_id, start_time, type, location, tag, frequency, is_mention, advance_mention_time) VALUES ";
        cout << "(" << i << ", 'activity_" << i << "', " << i << ", '2023-06-08 09:00:00', " << i % 10 << ", 'location_" << i << "', 'tag_" << i << "', " << i % 5 + 1 << ", " << i % 2 << ", " << i % 60 << ");"<<endl;
        
    }
    
    return 0;
}