#include"Interpreter.h"
static bool hassinglequote = 0;
static bool hasdoublequote = 0;

string Interpreter::getNextString(string s, int *pos){
    string word,tempstring;
    int BeginPos, EndPos;
    
    if (hassinglequote||hasdoublequote) {
        BeginPos = *pos;
    }
    else {
        while ((s[*pos] == ' ' || s[*pos] == '\n' || s[*pos] == '\t' || s[*pos] == '\r' || s[*pos] == 0) && s[*pos] != '\0') {
            (*pos)++;
        }
        BeginPos = *pos;
    }

	
	tempstring = s.substr(BeginPos,2);
	if(tempstring=="¡®"||tempstring=="¡¯"){
		word="ERROR_IME";
		return word;
	}
	

    if (hassinglequote) {
        while (s[*pos] != '\'' && (*pos) < s.size()) {
            (*pos)++;
        }
        EndPos = *pos;
        if (s[*pos] == '\'')hassinglequote = 0;
        else {
            word = "ERROR_MARKED";
            return word;
        }
        if (EndPos == BeginPos) {
            word = "";
            return word;
        }
        word = s.substr(BeginPos, EndPos - BeginPos);
        return word;
    }
    if (hasdoublequote) {
        while (s[*pos] != '\"' && (*pos) < s.size()) {
            (*pos)++;
        }
        EndPos = *pos;
        if (s[*pos] == '\"')hasdoublequote = 0;
        else {
            word = "ERROR_MARKED";
            return word;
        }
        if (EndPos == BeginPos) {
            word = "";
            return word;
        }
        word = s.substr(BeginPos, EndPos - BeginPos);
        return word;
    }
    if(s[*pos]=='(' || s[*pos]==',' || s[*pos]==')' || s[*pos]==';' || s[*pos]=='\'' || s[*pos]=='*'||s[*pos]=='\"'){
        EndPos=++(*pos);
        word = s.substr(BeginPos,EndPos-BeginPos);
        return word;
    }
    else{
        while (s[*pos] != ' ' && s[*pos] != '\n' && s[*pos] != '\t' && s[*pos] != '\r' && s[*pos] != '(' && s[*pos] != ',' && s[*pos] != ')' && s[*pos] != 0 && s[*pos] != '\'' && s[*pos] != ';' && s[*pos] != '\0' && s[*pos] != '*') {
            (*pos)++;
        }
        EndPos=*pos;
        if(EndPos==BeginPos){
            word="";
            return word;
        }
        word=s.substr(BeginPos,EndPos-BeginPos);
        return word;
    }
}


bool Interpreter::isWord(string s){
    if(s[0]=='\'' || s[0]==';' || s[0]==',' || s[0]=='(' || s[0]==')'){
        return false;
    }
    return true;
}

int Interpreter::mainInterpreter(string s){
    string word;
    int pos=0;
    word=getNextString(s,&pos);

    if(word=="create"){
    	
        int cntbreak=0;
        word=getNextString(s,&pos);

        if(word=="table"){
            string primarykey="";
            string tablename="";
            word=getNextString(s,&pos);
            if(!isWord(word)){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            tablename=word;

            word=getNextString(s,&pos);
            if(word!="("){
                cout<<"ERROR: invalid query format!"<< endl;
                return ERROR_SYNTAX;
            }

            word=getNextString(s,&pos);
            vector<Attribute> attrList;
            
            while(!word.empty() && word!="primary" && word!=")"){
            	
                cntbreak++;
                string attrName = word;
                
                int type;
                bool unique=false;
                word=getNextString(s,&pos);
                if(word=="int"){
                    type=TYPE_INT;
                }else if(word=="float"){
                    type=TYPE_FLOAT;
                }else if(word=="char"){
                    word=getNextString(s,&pos);
                    if(word!="("){
                        cout<<"ERROR: invalid query format!"<<endl;
                        return ERROR_SYNTAX;
                    }
                    word=getNextString(s,&pos);
                    stringstream ss;
                    ss << word;
                    if(!(ss>>type)){
                        ss<<"";
                        cout<<"ERROR: invalid query format!"<<endl;
                        return ERROR_SYNTAX;
                    }
                    ss<<"";
                    word=getNextString(s,&pos);
                    
                    if(word!=")"){
                        cout<<"ERROR: invalid query format! "<<endl;
                        return ERROR_SYNTAX;
                    }
                }else{
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
                }
                word=getNextString(s,&pos);
                if(word=="unique"){
                	unique=true;
                	word=getNextString(s,&pos);
				} 
                if(word=="primary"){
                	word=getNextString(s,&pos);
                	if(word!="key"){
                		cout<<"ERROR: invalid query format!"<<endl;
						return ERROR_SYNTAX;
					}
					unique=true;
					primarykey=attrName;
					word=getNextString(s,&pos);
				}
                Attribute attr(attrName,type,unique);
                attrList.push_back(attr);
                if(word!="," && word!=")" && !word.empty()){
                    cout<<"ERROR: invalid query format!" <<endl;
                    return ERROR_SYNTAX;
                }
				else if(word==")"){
                    break;
                }
				else if(cntbreak>=100){
                    cout<<"ERROR: invalid query format!"<<endl;
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
                }
                word=getNextString(s,&pos);
                if(!isWord(word)){
                	cout<<"ERROR: invalid query format!"<<endl;
                	return ERROR_SYNTAX;
				}
            }

                
            if(word=="primary"){
                word=getNextString(s,&pos);
                if(word!="key"){
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
                }
                word=getNextString(s,&pos);
                if(word!="("){
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
				}            
                word=getNextString(s,&pos);
                primarykey=word;
                int i;
                for(i=0;i<attrList.size();i++){
                    if(primarykey==attrList[i].name){
                        attrList[i].ifUnique=true;
                        break;
                    }
                }
                if(i==attrList.size()){
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
                }
                word=getNextString(s,&pos);
                if(word!=")"){
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
                }
            }

            if(word!=")"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            api->CreateTable(tablename,&attrList,primarykey);
			//cout << "Interpreter: successful create!" << endl;
            return TRUE_SYNTAX;
        }
        
        else if(word=="index"){
            string indexname="";
            string tablename="";
            string attrname="";
            word=getNextString(s,&pos);
            if(!isWord(word) || word=="on"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            indexname=word;
            word=getNextString(s,&pos);
            if(word!="on"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            word=getNextString(s,&pos);
            if(!isWord(word)){
                cout<<"ERROR: expect a table name in create index"<<endl;
                return ERROR_SYNTAX;
            }
            tablename=word;
            word=getNextString(s,&pos);
            if(word!="("){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            word=getNextString(s,&pos);
            if(!isWord(word) || word.empty()){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            attrname=word;
            word=getNextString(s,&pos);
            if(word!=")"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            api->CreateIndex(indexname,tablename,attrname);
			//cout << "Interpreter: successful create!" << endl;
            return TRUE_SYNTAX;
        }
        else{
            cout<<"ERROR: invalid query format!"<<word<<endl;
            return ERROR_SYNTAX;
        }
    }

    else if(word=="drop"){
        string tablename;
        string indexname;
        word=getNextString(s,&pos);
        if(word=="table"){
            word=getNextString(s,&pos);
            if(!isWord(word)){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            tablename=word;
            api->DropTable(tablename);
			//cout << "Interpreter: successful drop!" << endl;
            return TRUE_SYNTAX;
        }
        else if(word=="index"){
            word=getNextString(s,&pos);
            if(!isWord(word)){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            indexname=word;
            api->DropIndex(indexname);
			//cout << "Interpreter: successful drop!" << endl;
            return TRUE_SYNTAX;
        }
        else{
            cout <<"ERROR: invalid query format!"<<word<<endl;
        }
    }

    else if(word=="insert"){
        int cntbreak=0;
        string tablename="";
        vector<string>valueList;
        word=getNextString(s,&pos);
        if(word!="into"){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        word=getNextString(s,&pos);
        if( word.empty() || word=="values" || !isWord(word)){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        tablename=word;
        word=getNextString(s,&pos);
        if(word!="values"){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        word=getNextString(s,&pos);
        if(word!="("){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        word=getNextString(s,&pos);
        while(word!=")"){
            cntbreak++;
            if(word=="ERROR_IME"){
            	cout<<"ERROR: invalid query format!"<<endl;
				return ERROR_SYNTAX; 
			} 
            if(word=="\'"||word=="\""){
                if(word == "\'")hassinglequote = 1;
                if (word == "\"")hasdoublequote = 1;
                word=getNextString(s,&pos);
            }
            if (word == "ERROR_MARKED") {
                cout << "ERROR: invalid query format!" << endl;
                return ERROR_SYNTAX;
            }
            valueList.push_back(word);
            word=getNextString(s,&pos);
            if(word=="ERROR_IME"){
            	cout<<"ERROR: invalid query format!"<<endl;
				return ERROR_SYNTAX; 
			} 
            if(word=="\'" || word == "\""){
                word=getNextString(s,&pos);
            }
            if(word==","){
                word=getNextString(s,&pos);
                if(word==")"){
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
                }
            }
            else if(word!="," && word!=")" && !word.empty()){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            if(cntbreak>=100){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
        }

        api->InsertRecord(tablename,&valueList);
		//cout << "Interpreter: successful insert!" << endl;
        return TRUE_SYNTAX;
    }

    else if(word=="delete"){
        int cntbreak=0;
        string tablename="";
        word=getNextString(s,&pos);
        if(word!="from"){
            cout << "ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        word=getNextString(s,&pos);
        if(word.empty()||word=="where"){
            cout<< "ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        tablename=word;
        word=getNextString(s,&pos);
        if(word==";"||word.empty()){
            api->DeleteRecord(tablename);
			cout << "Interpreter: successful delete!" << endl;
            return TRUE_SYNTAX;
        }
        if(word!="where"){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        string attrname="";
        string value="";
        vector<Condition> conditionList;
        int op=0;
        word=getNextString(s,&pos);
        while(true){
            cntbreak++;
            if(word.empty()||word==";"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            attrname=word;
            word=getNextString(s,&pos);
            if(word=="=")op=OP_EQUAL;
            else if(word=="<")op=OP_LESS;
            else if(word==">")op=OP_GREATER;
            else if(word==">=")op=OP_GREATER_EQUAL;
            else if(word=="<=")op=OP_LESS_EQUAL;
            else if(word=="<>")op=OP_NOT_EQUAL;
            else{
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            word=getNextString(s,&pos);
            if(word=="ERROR_IME"){
            	cout<<"ERROR: invalid query format!"<<endl;
				return ERROR_SYNTAX; 
			} 
            if(word=="\'" || word == "\""){
                if (word == "\'")hassinglequote = 1;
                if (word == "\"")hasdoublequote = 1;
                word=getNextString(s,&pos);
            }
            if(word.empty() || word=="and" || !isWord(word)){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            value=word;
            word=getNextString(s,&pos);
            if(word=="ERROR_IME"){
            	cout<<"ERROR: invalid query format!"<<endl;
				return ERROR_SYNTAX; 
			} 
            if(word=="\'" || word == "\""){
                word=getNextString(s,&pos);
            }
            Condition cdt(attrname,op,value);
            conditionList.push_back(cdt);
            if(word==";" || word.empty()){
                break;
            }else if(cntbreak>100){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            else if (word!="and"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            word=getNextString(s,&pos);
        }
        api->DeleteRecord(tablename,&conditionList);
		cout << "Interpreter: successful delete!" << endl;
        return TRUE_SYNTAX;
    }

    else if(word=="select"){
        int cntbreak=0;
        vector<string> attrSelect;
        string tablename="";
        word=getNextString(s,&pos);
        
        if(word!="*"){
			if(word=="from" || word.empty()){
        		cout<<"ERROR: invalid query format!"<<endl;
        		return ERROR_SYNTAX;
			}                          
            while(word!="from"){
                cntbreak++;
                attrSelect.push_back(word);
                word=getNextString(s,&pos);
                if(word==","){
                    word=getNextString(s,&pos);
                }
                if(cntbreak>100){
                    cout<<"ERROR: invalid query format!"<<endl;
                    return ERROR_SYNTAX;
                }
            }
        }
        else if(word=="*"){
            word=getNextString(s,&pos);
        }
        if(word!="from"){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        word=getNextString(s,&pos);
        if(word=="where" || !isWord(word) || word.empty()){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        tablename=word;
        word=getNextString(s,&pos);
        if(word.empty() || word==";"){
            if(attrSelect.size()==0){
                api->SelectRecord(tablename);
            }else{
                api->SelectRecord(tablename,&attrSelect);
            }
			//cout << "Interpreter: successful select!" << endl;
            return TRUE_SYNTAX;
        }
        if(word!="where"){
            cout<<"ERROR: invalid query format!"<<endl;
            return ERROR_SYNTAX;
        }
        cntbreak=0;
        string attrname="";
        string value="";
        vector<Condition> conditionList;
        int op=0;
        word=getNextString(s,&pos);
        while(true){
            cntbreak++;
            if(word.empty()||word==";"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            attrname=word;
            word=getNextString(s,&pos);
            if(word=="=")op=OP_EQUAL;
            else if(word=="<")op=OP_LESS;
            else if(word==">")op=OP_GREATER;
            else if(word==">=")op=OP_GREATER_EQUAL;
            else if(word=="<=")op=OP_LESS_EQUAL;
            else if(word=="<>")op=OP_NOT_EQUAL;
            else{
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            word=getNextString(s,&pos);
            if(word=="ERROR_IME"){
            	cout<<"ERROR: invalid query format!"<<endl;
				return ERROR_SYNTAX; 
			} 
            if(word=="\'" || word == "\""){
                if (word == "\'")hassinglequote = 1;
                if (word == "\"")hasdoublequote = 1;
                word=getNextString(s,&pos);
            }
            if(word.empty()){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            value=word;
            word=getNextString(s,&pos);
            if(word=="ERROR_IME"){
            	cout<<"ERROR: invalid query format!"<<endl;
				return ERROR_SYNTAX; 
			} 
            if(word=="\'" || word == "\""){
                word=getNextString(s,&pos);
            }
            Condition cdt(attrname,op,value);
            conditionList.push_back(cdt);
            
            if(word==";" || word.empty()){
                break;
            }else if(cntbreak>100){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            else if (word!="and"){
                cout<<"ERROR: invalid query format!"<<endl;
                return ERROR_SYNTAX;
            }
            word=getNextString(s,&pos);
        }
        if(attrSelect.size()==0){
            api->SelectRecord(tablename,NULL,&conditionList);
        }else{
            api->SelectRecord(tablename,&attrSelect,&conditionList);
        }
		//cout << "Interpreter: successful select!" << endl;
        return TRUE_SYNTAX;
    }
    else if(word=="quit"){
        return QUIT;
    }else if(word=="execfile"){
        FileName=getNextString(s,&pos);
		fstream file;
		//Interpreter ip(&api);
		int state;
		file.open(FileName, ios::in);
		if (!file.is_open()) {
			cout << "No file named " << FileName << endl;
			state = TRUE_SYNTAX;
			//continue;
		}
		while (getline(file, s, ';')) {
			state = mainInterpreter(s);
		}
		file.close();
		state = TRUE_SYNTAX;
		return state;
    }else{
    	if(!word.empty()){
      	    cout<<"ERROR: invalid query format! "<< endl;
            return ERROR_SYNTAX;
        }else{
        	return TRUE_SYNTAX;
		}
    }
}
