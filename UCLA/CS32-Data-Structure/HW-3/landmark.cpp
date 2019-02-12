//
//  landmark.cpp
//  HM3
//
//  Created by ali mirabzadeh on 2/5/19.
//  Copyright © 2019 AMZ Development. All rights reserved.

//constraints
//You must not declare any struct or class other than Landmark, Hotel, Restaurant, and Hospital.
//The Landmark class must not have a default constructor. The only constructor you may declare for Landmark must have exactly one parameter. That parameter must be of type string, and it must be a useful parameter.
//Although the expression new Restaurant("Bruin Bite",30) is fine, the expression new Landmark("La Picadura del Oso") must produce a compilation error. (A client can create a particular kind of landmark object, like a Restaurant, but is not allowed to create an object that is just a plain Landmark.)
//Other than constructors and destructors (which can't be const), all member functions must be const member functions.
//No two functions with non-empty bodies may have the same implementation, or implementations that have the same effect for a caller. For example, there's a better way to deal with the name() function than to have each kind of landmark declare and identically implement a name function. (Notice that { return "shopping cart"; } and { return "purse"; } do not have the same effect, but { return "yellow"; } and { string s("yell"); return s + "ow"; } have the same effect, which is to return "yellow".
//No implementation of a name() function may call any other function.
//No class may have a data member whose value is identical for every object of a particular class type.
//All data members must be declared private. You may declare member functions public or private. Your solution must not declare any protected members (which we're not covering in this class). Your solution must not contain the word friend.


//All the classes got implemented by me
//ABC class
class Landmark
{
    public:
        //Landmark constructor
        Landmark(string name)
        {
            m_name=name;
        }
        //non-virtual function
        virtual const string name() const
        {
            return m_name;
        }
        //pure virtual functions in the base class
        virtual string color() const=0;
        virtual string icon() const=0;
        //virtual base destructor
        virtual ~Landmark(){};
    private:
        string m_name;
    
};
//derived class: Hotel
class Hotel: public Landmark
{
    public:
        Hotel(string name):Landmark(name)
        {}
        virtual string color() const
        {
            return "yellow";
        }
        virtual string icon() const
        {
            return "bed";
        }
        ~Hotel()
        {
            cout << "Destroying the hotel " << name() << endl;
        }
};

class Restaurant: public Landmark
{
    public:
        Restaurant(string name, int maxCap):Landmark(name)
        {
            m_RmaxCap=maxCap;
        }
        virtual  string color() const
        {
            return "yellow";
        }
        virtual string icon() const
        {
            if (m_RmaxCap<40)
                return "small knife/fork";
            else if(m_RmaxCap>-40)
                return "large knife/fork";
            else
                return "";
        }
        ~Restaurant()
        {
            cout << "Destroying the restaurant " << name() << endl;
        }
    private:
        int m_RmaxCap;
};

class Hospital: public Landmark
{
    public:
        Hospital(string name):Landmark(name)
        {
        }
        virtual string color() const
        {
            return "blue";
        }
        virtual string icon() const
        {
            return "H";
        }
        ~Hospital()
        {
            cout << "Destroying the hospital " << name() << endl;
        }
};
//Output expected

//Here are the landmarks.
//Display a yellow bed icon for Westwood Rest Good.
//Display a yellow small knife/fork icon for Bruin Bite.
//Display a yellow large knife/fork icon for La Morsure de l'Ours.
//Display a blue H icon for UCLA Medical Center.
//Cleaning up.
//Destroying the hotel Westwood Rest Good.
//Destroying the restaurant Bruin Bite.
//Destroying the restaurant La Morsure de l'Ours.
//Destroying the hospital UCLA Medical Center.
