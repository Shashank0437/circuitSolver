#include <simplecpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Button
{
    int x,y,w,h;
    const char* str;
    Button(const char* name, int xx,int yy,int ww,int hh)
    {
        x=xx;
        y=yy;
        w=ww;
        h=hh;
        str=name;
        Rectangle rec1(x,y,w,h);
        rec1.setFill();
        rec1.setColor(COLOR(255,0,255));
        rec1.imprint();

        Text(x,y,name).imprint();
    }
    void set_but()
    {
        Rectangle rec(x,y,w,h);
        rec.setFill();
        rec.setColor(COLOR(0,255,255));
        rec.imprint();
        Text(x,y,str).imprint();
    }

    void clr_but()
    {
        Rectangle rec(x,y,w,h);
        rec.setFill();
        rec.setColor(COLOR(255,0,255));
        rec.imprint();
        Text(x,y,str).imprint();
    }

    void sol_but()
    {
        Rectangle rec(x,y,w,h);
        rec.setFill();
        rec.setColor(COLOR(255,0,0));
        rec.imprint();
        Text(x,y,str).imprint();
    }

    bool in(int click)
    {
        int cx=click/65536 ,cy=click % 65536;
        return(abs(cx-x)<=w && abs(cy-y)<=h);

    }

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MathRep
{
    vector<vector<double> > A;
    vector<double>b;


    void addNode()
    {
        A.push_back(vector<double>(A.size(),0));
        for(size_t i =0; i<A.size(); i++)
            A[i].push_back(0);
        b.push_back(0);
    }

    void addSource(int i,int j,double val)
    {
        b[i]-=val;
        b[j]+=val;
    }

    void addConductance(int i,int j,double val)
    {
        A[i][i]+=val;
        A[j][j]+=val;
        A[i][j]-=val;
        A[j][i]-=val;
    }

    void solve()
    {
        for(size_t i=0; i<A.size()-1; i++)
        {
            for(size_t j=0; j<A.size()-1; j++)
            {
                if(i!=j)
                {
                    double mplier = A[j][i]/A[i][i];
                    for(size_t k=0; k<A.size()-1; k++)
                        A[j][k]-=A[i][k]*mplier;
                    b[j]=b[j]-b[i]*mplier;
                }
            }
        }
        cout<< "printing node potentials:"<<endl;
        for(size_t i=0; i<A.size()-1; i++)
            cout <<i<<":"<<b[i]/A[i][i] << endl;
        cout<<A.size()-1<<":"<<0<<endl;

    }

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Node
{

    double x,y;
    Node(int click,int index)
    {
        x=click/65536;
        y=click%65536;
        Circle(x,y,20).imprint();
        Text(x,y,index).imprint();
    }
    bool in(int click)
    {
        return pow((x- click/65536),2) + pow((y- click%65536),2) < pow(20,2);
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CanvasContent
{
    vector<Node*>Nptr;
    MathRep *mrp;

    CanvasContent(MathRep *mrp0)
    {
        mrp=mrp0;
    }

    void addNode()
    {
        int vclick=getClick();
        Nptr.push_back(new Node(vclick,Nptr.size()));
        mrp->addNode();
    }

    //void deleteNode()
    //{
      //  double nx=Nptr[Nptr.size()-1]->x;
      //  double ny=Nptr[Nptr.size()-1]->y;

      //  Circle newcir(nx,ny,20);
      //  newcir.setFill();
       // newcir.setColor(COLOR(255,255,255));
       // newcir.imprint();

       // Nptr.pop_back();
      //  cout<<"done";
    //}

    int selectNode()
    {
        int click=getClick();
        for(size_t i=0; i<Nptr.size(); i++)
            if(Nptr[i]->in(click)) return i;
        return -1;
    }


    void addConductance()
    {
        int i1=selectNode(),i2=selectNode();
        if(i1>=0 &&  i2>=0)
        {
            double x1=Nptr[i1]->x , y1=Nptr[i1]->y,x2=Nptr[i2]->x ,y2=Nptr[i2]->y ;
            double dx=x2-x1 , dy=y2-y1 ,p1=x1,q1=y1;
            for(int m=1; m<10; m++)
            {
                Line(p1,q1,p1+ dx/10,q1).imprint();
                Line(p1+ dx/10,q1,p1+ dx/10,q1 + dy/10).imprint();
                p1=p1 + dx/10 ;
                q1=q1 + dy/10;
            }

            cout<<"Conductance Value";
            double g;
            cin>>g;
            Text((x1+x2)/2 - 6 , (y1+y2)/2 - 6,g).imprint();
            mrp->addConductance(i1,i2,g);
        }
        else cout<<"Select operation again."<<endl;
    }

    void addSource()
    {
        int i1=selectNode(),i2=selectNode();
        if(i1>=0 &&  i2>=0)
        {
            double x1=Nptr[i1]->x , y1=Nptr[i1]->y,x2=Nptr[i2]->x ,y2=Nptr[i2]->y ;
            Line l(x1,y1,x2,y2);

            l.setColor(COLOR(255,0,0));


            l.imprint();

            cout<<"Current Source Strength";
            double J ;
            cin>>J;
            mrp->addSource(i1,i2,J);
            Text((x1+x2)/2 , (y1+y2)/2,J).imprint();
            Turtle t;
            t.penUp();
            t.moveTo((x1+x2)/2 , (y1+y2)/2);
            t.face(x2,y2);
            t.forward(10);
            t.imprint();
        }
        else cout<<"Select operation again."<<endl;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main()
{
    initCanvas("Circuit editor",600,600);
    for(int z=0; z<5; z++)
    {
        Rectangle recc(300,300,570+ 5*z,570 +5*z);
        recc.setColor(COLOR(0,102,0));
        recc.imprint();
    }
    Button b1("Node",100,50,160,30);
    Button b2("Conductance ", 100,150,160,30);
    Button b3("Source", 100,250,160,30);
    Button b4("Solve", 100,350,160,30);
    Button b5("Quit", 100,450,160,30);
    Button b6("Undo", 100,550,160,30);

    MathRep mr;
    CanvasContent cc(&mr);

    while(true)
    {
        int cpoint=getClick();
        if     (b1.in(cpoint))
        {
            b1.set_but();
            cc.addNode();
            b1.clr_but();
        }
        else if(b2.in(cpoint))
        {
            b2.set_but();
            cc.addConductance();
            b2.clr_but();

        }
        else if(b3.in(cpoint))
        {
            b3.set_but();
            cc.addSource();
            b3.clr_but();
        }
        else if(b4.in(cpoint))
        {
            b4.sol_but();
            mr.solve();
            b4.clr_but();
        }
        //else if(b6.in(cpoint))
       // {
        //    b6.set_but();
        //    cc.deleteNode();
        //    b6.clr_but();
       // }

        else if(b5.in(cpoint))
        {
            b5.sol_but();
            delay(1000);
            break;
        }
    }
}
