#include <string>
#include <vector>
#include <algorithm>    // std::for_each
#include <iostream>

static void printItem (const std::string& val) {  // function:
  std::cout << "Item: " << val << std::endl;
}


int foreachSample() {
	std::vector<std::string> menu_list_down(3);

	menu_list_down[1] = "one";
	menu_list_down[2] = "two";

	for_each (menu_list_down.begin(), menu_list_down.end(), printItem);

	return 0;
}
