#include<iostream>
#include<functional>
using namespace std;
class A{
public:
        A(int x=0):a(x){}
        int a;
        bool operator<(const A& ref) const{
                return a<ref.a;
        }
};
int lessmain()
{
        A a1(1);
        A a2(2);
        std::less<A> comp;
        if( comp(a1,a2)){
                cout<<"less"<<endl;
        }
        else{
                cout<<"more"<<endl;
        }

        return 0;
}
