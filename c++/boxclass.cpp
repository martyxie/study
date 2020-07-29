
#include <iostream>
using namespace std;

class box
{
    public:
        static int m_test;
        int get_box_soc(void);
        void set_box_soc(int len,int with,int higth);
        void print_box_info(void);
        friend void get_test(box *x);
        //运算符重载
        box operator+(const box& b);
        box();
        ~box();
    private:
        int length;
        int with;
        int higth;
};
//构造函数,在一个类创建时调用,该函数可以对类成员做一些初始化
box::box(void)
{
    cout << "create box" << endl;
    length = 100;
    with = 100;
    higth = 100;
}
//析构函数,删除一个对象时调用,一般是释放资源
box::~box(void)
{
    cout << "delete box" << endl;
}

int box::get_box_soc(void)
{
    int ret = length + with + higth;
    cout << ret << endl;
    return  ret;
}

void box::set_box_soc(int len,int w,int h)
{
    cout << "set box data" << endl;
    length = len;
    with = w;
    higth = h;
    return ;
}
void box::print_box_info(void)
{
    cout << "length = "<< length << endl;
    cout << "with = " << with << endl;
    cout << "higth = " << higth << endl;
    return;
}

box box::operator+(const box& b)
{
    box box;
    box.length = this->length + b.length;
    box.with = this->with + b.with;
    box.higth = this->higth + b.higth;
    return box;
}

// 继承(nbox 继承了 box,即 nbox 为box的派生类，基类为 box)
class nbox:public box
{
    public:
        int get_box_soc(void);
        int set_nbox_soc(int m);
        void set_box_soc(int len,int with,int higth);
    private:
        int mac;
};

int nbox::set_nbox_soc(int m)
{
    mac = m;
}
int nbox::get_box_soc(void)
{
    return mac ;
}
void nbox::set_box_soc(int len,int with,int higth)
{
    cout<< "nbox:set_box_soc" << endl;
    box::set_box_soc(len,with,higth);
}
// 友元函数/类，不属于类的成员，但可以访问类的所有成员
void get_test(box *p)
{
    cout << p->length << endl;
    cout << p->with << endl;
    cout << p->higth << endl;
}

int main(void)
{
    box *pbox = new box;
    if(pbox)
        pbox->get_box_soc();
    nbox newbox;
    if(pbox)
    {
        pbox->set_box_soc(10,4,5);
        cout << pbox->get_box_soc()<<endl;
        get_test(pbox);
        delete pbox;
    }
    newbox.set_box_soc(10,6,9);
    newbox.set_nbox_soc(1000);
    cout << newbox.get_box_soc() << endl;


    cout << "test operator +" << endl;
    box *box1 = new box;
    box *box2 = new box;
    if(box1== NULL || box2 == NULL)
    {
        cout << "new box error" << endl;
        return -1;
    }
    box1->set_box_soc(1,2,3);
    box1->print_box_info();
    box2->set_box_soc(5,6,7);
    box2->print_box_info();
    box box3 = *box1 + *box2;
    delete box1;
    delete box2;
    box3.print_box_info();
    return 0;
}
