// Design Facebook APIs (Create Post, Delete Post, Get Feed, Get Feed Paginated, Follow, Unfollow). Machine coding round with few test cases around it which you need to clear. Use of 1-2 design patterns is recommended

#include <bits/stdc++.h>
using namespace std;

bool isloggedin=0;

class User{
    private: 
        int age;
        string gender;
        string passwordhash;
        set<string> following;
        set<string> followers;
        set<int> postids;
    public:
        string username;
        User(string username, int age, string gender, string password): username(username), gender(gender), age(age), passwordhash(passhashgenerator(password)){
            if(age<1){
                throw invalid_argument("age cannot be negative.\n");
            }
            if(password.length()<8){
                throw invalid_argument("password too short");
            }
        }
        string passhashgenerator(string password){
            string passhash="";
            for(int i=1;i<password.length();i++){
                char a=password[i-1]*password[i]%256;
                passhash.push_back(a);
            }
            return passhash;
        }
        void addfollowee(string followname){
            following.insert(followname);
        }
        void addfollower(string followname){
            followers.insert(followname);
        }
        bool userlogin(string password){
            return (passhashgenerator(password)==passwordhash);
        }
        void getfollowers(){
            for(auto f:followers){
                cout<<f+" ";
            }
            cout<<endl;
        }
        void addpost(int id){
            postids.insert(id);
        }
        set<int> allposts(){
            return postids;
        }
        void removefollowee(string followee){
            following.erase(followee);
        }
        void removefollower(string follower){
            followers.erase(follower);
        }
};

class UserManager{
    private:
        map<string, User*> members;
        pair<bool,string> loginuser={false,""};
    public:
    string curruser(){
        return loginuser.second;
    }
    bool adduser(string username, int age, string gender, string password){
        if(members.find(username)==members.end()){
            try
            {
                User* newuser= new User(username, age, gender, password);
                members[username]=newuser;
                loginuser={true, username};
                cout<<"welcome to the platform!"<<endl;
                return true;
            }
            catch(const exception& e)
            {
                cerr << e.what() << '\n';
            }
        }
        cout<<"username already exists!"<<endl;
        return false;
    }
    bool login(string username, string password){
        if(members.count(username)!=0){
            if(members[username]->userlogin(password)){
                cout<<"login successful user "+ username<<endl;
                loginuser={true,username};
                return true;
            }
            else{
                cout<<"wrong password\n";
            }
        }
        else{
            cout<<"wrong username\n";
        }
        return false;
    }
    bool addfollower(string followee){
        if(loginuser.first && members.count(followee)==0){
            return false;
        }
        members[loginuser.second]->addfollowee(followee);
        members[followee]->addfollower(loginuser.second);
        cout<<"followed successfully!"<<endl;
        return true;
    }
    bool unfollow(string followee){
        if(loginuser.first && members.count(followee)==0){
            return false;
        }
        members[loginuser.second]->removefollowee(followee);
        members[followee]->removefollower(loginuser.second);
        cout<<"unfollowed successfully!"<<endl;
        return true;
    }
    void viewfollowers(){
        members[loginuser.second]->getfollowers();
    }
    void userpost(int id){
        members[loginuser.second]->addpost(id);
    }
    void logout(){
        loginuser={false, ""};
    }
    void deleteaccount(){
        delete(members[loginuser.second]);
        members.erase(loginuser.second);
        loginuser={false,""};
    }
    set<int> viewuserposts(){
        return members[loginuser.second]->allposts();
    }
};

class Post{
    private:
        string username;
        int id;
        string postcontent;
        int likes;
    public:
        Post(string user, string content, int id): username(user), postcontent(content), likes(0), id(id) {}
        void likepost(){
            likes++;
        }
        string getpostcontent(){
            return postcontent;
        }
        int getlikes(){
            return likes;
        }
};

class PostManager
{
private:
    map<int, Post*> allposts;
    int id=1;
public:
    int createpost(string content, string username){
        Post* newpost=new Post(username, content, id);
        allposts[id]=newpost;
        id++;
        cout<<"post created successfully!"<<endl;
        return id-1;
    }
    void viewposts(set<int> postid){
        for(auto p:postid){
            cout<< allposts[p]->getpostcontent() + "\nlikes " + to_string(allposts[p]->getlikes())<<endl;
        }
    }
};
PostManager pm;
UserManager um;
int main(){
    int a=0;
    while(true){
    if(a==0){
    cout<<"Hello and welcome to FB!\n 1. Sign in \n 2. Login \n 3. End"<<endl;
    int b;
    cin>>b;
    if(b==1){
        string username, gender, password;
        int age;
        cout<<"username?\n";
        cin>>username;
        cout<<"age?\n";
        cin>>age;
        cout<<"gender?\n";
        cin>>gender;
        cout<<"password?\n";
        cin>>password;
        if(um.adduser(username, age, gender, password)){
            a=1;
        }
        continue;
    }
    else if(b==2){
        string username, password;
        cout<<"username?\n";
        cin>>username;
        cout<<"password?\n";
        cin>>password;
        if(um.login(username, password)){
            a=1;
        }
        continue;
    }
    else if(b==3){
        return 0;
    }
    else{
        continue;
    }
    }
    else if(a==1){
    cout << "1. Create Post\n"
         << "2. View Posts\n"
         << "3. Follow User\n"
         << "4. Unfollow User\n"
         << "5. View Followers\n"
         << "6. Logout\n"
         << "7. Delete Account\n"
         << "8. Exit" << endl;
    int choice;
    cin >> choice;
    if (choice == 1) {
        string content;
        cout << "Enter post content:\n";
        cin.ignore();
        getline(cin, content);
        um.userpost(pm.createpost(content, um.curruser()));
    } else if (choice == 2) {
        pm.viewposts(um.viewuserposts());
    } else if (choice == 3) {
        string followee;
        cout << "Enter username to follow:\n";
        cin >> followee;
        um.addfollower(followee);
    } else if (choice == 4) {
        string followee;
        cout << "Enter username to unfollow:\n";
        cin >> followee;
        um.unfollow(followee);
    } else if (choice == 5) {
        um.viewfollowers();
    } else if (choice == 6) {
        um.logout();
        a = 0;
    } else if (choice == 7) {
        um.deleteaccount();
        a = 0;
    } else if (choice == 8) {
        return 0;
    } else {
        cout << "Invalid choice. Please try again.\n";
    }
    }
    }
}