#include <Arduino.h>
#include <Global_vars.h>



boolean has_condition_type1=false;
boolean has_condition_type2=false;
boolean has_condition_type3=false;
boolean receive_condition_type1=false;
boolean receive_condition_type2=false;
boolean receive_condition_type3=false;
String code_str_condition_type1;
String code_str_condition_type2;
String code_str_condition_type3;

boolean process_based_on_OID_coordinates=false;


String remove_delimiters(String str)
{
    String clean_str = "";
    int code_item_counter = 0;
    for (int i = 0; i < (str.length()); i++)
    {
        char c = str.charAt(i);
        if (c != ';')
        {
            clean_str += c;
        }
        else if (c == ';')
        {
            code_item_counter++;
        }
    }
    Serial.println(clean_str);
    Serial.print("counter: ");
    Serial.println(code_item_counter);
    if (code_item_counter > commands_size)
    {
        clean_str = "Oversize";
    }
    return clean_str;
}

int check_condition(char condition_symbol_start,char condition_symbol_end, String str){
    //0 没有找到符号
    //1找到符号并且存到对应的code_str_condition_type中并且has_condition_type标志被转为true，同时剔除掉在code_str_clean的部分
    //2语法错误
    int positive_bracket=str.indexOf(condition_symbol_start);
    int negative_bracket=str.indexOf(condition_symbol_end);
    if(positive_bracket==-1&&negative_bracket==-1){
        if(condition_symbol_start=='{'){
            has_condition_type1=false;
            Serial.println("no condition {");
        }else if(condition_symbol_start=='['){
            has_condition_type2=false;
            Serial.println("no condition [");
        }else if(condition_symbol_start=='<'){
            has_condition_type3=false;
            Serial.println("no condition <");
        }
        return 0;//没有条件标识符
    }
    else if((positive_bracket>=0&&negative_bracket==-1)||(positive_bracket==-1&&negative_bracket>=0)){
        Serial.println("条件标识符不完整,语法错误！");
        return 2;//只有一半的条件标识符 has'{'no'}' has'['no']' has'<'no'>'
    }
    else if(positive_bracket>=0&&negative_bracket>=0){
        Serial.println("也许是good code,继续检查");
        int positive_bracket_counter=0;
        int negative_bracket_counter=0;
        for(int i=0;i<str.length();i++){
            if(str[i]==condition_symbol_start)positive_bracket_counter++;
            if(str[i]==condition_symbol_end)negative_bracket_counter++;
        }
        if(positive_bracket_counter!=1||negative_bracket_counter!=1){
            Serial.println("标识符过多，语法错误");
            return 2;
        }
        else{
            if(positive_bracket>=negative_bracket){
                Serial.println("条件开始标识符在结束提示符之后，语法错误");
                return 2;
            }else{
                String temp_condition_str = str.substring(positive_bracket+1,negative_bracket);
                Serial.print("条件语句: ");Serial.println(temp_condition_str);
                //这里很重要！我们该把裁剪出来的条件语句放到哪个type下的全局变量存储？目前{}是type1，[]是type2,||是type3，后续还可以添加，记得改这里
                if(condition_symbol_start=='{'){
                    code_str_condition_type1=temp_condition_str;
                    has_condition_type1=true;
                }else if(condition_symbol_start=='['){
                    code_str_condition_type2=temp_condition_str;
                    has_condition_type2=true;
                }else if(condition_symbol_start=='<'){
                    code_str_condition_type3=temp_condition_str;
                    has_condition_type3=true;
                }
                code_str_clean.remove(positive_bracket,(negative_bracket-positive_bracket+1));
                Serial.print("去掉条件语句: ");
                Serial.println(code_str_clean);
                return 1;
            }
        }
    }else {
        return 2;//in dont know what the fuck happened;it should not be happenning;
    }

}
int legal_test_simple(String str){ //1-合法 0-不合法 2-超长
    int positive_bracket_counter = 0;
    int negetive_bracket_counter = 0;
    if (str.startsWith("Oversize"))
    {
        return 2;
    }
    else
    {
        if (str.length() < 2)
        {
            return 3; //还没有录入编程指令
        }
        else if (str.length() >= 2)
        {
            int bracket_counter = 0;
            for (int i = 0; i < (str.length()); i++)
            {
                char c = str.charAt(i);
                if (c == '(')
                {
                    bracket_counter++;
                    positive_bracket_counter++;
                }
                else if (c == ')')
                {
                    bracket_counter--;
                    negetive_bracket_counter++;
                }
                if (bracket_counter < 0)
                {
                    return 0;
                    break;
                }
            }
            if (positive_bracket_counter == negetive_bracket_counter)
            {
                return 1;
            }
            else
                return 0;
        }
        else
            return 0; //没有可能会发生这种情况，只是让计算机不报warning
    }
}

void check_code(String code_str_raw_local){
    Serial.println("Checking Code");
    Serial.println(code_str_raw_local);
    code_str_clean = remove_delimiters(code_str_raw_local);
    int check_condition_type1_result = check_condition('{', '}', code_str_clean); // 0-没有type1的条件判断符号{}, 1-正确（详细说明看checkcondition功能，2语法错误
    int check_condition_type2_result = check_condition('[', ']', code_str_clean); // 0-没有type2的条件判断符号[], 1-正确（详细说明看checkcondition功能，2语法错误
    int check_condition_type3_result = check_condition('<', '>', code_str_clean); // 0-没有type3的条件判断符号||, 1-正确（详细说明看checkcondition功能，2语法错误
    int legal_result=0;
    legal_result=legal_test_simple(code_str_clean);
    if (check_condition_type1_result == 1)legal_result = legal_test_simple(code_str_condition_type1);
    else if (check_condition_type1_result == 2)legal_result = 0; //语法错误
    if (check_condition_type2_result == 1)legal_result = legal_test_simple(code_str_condition_type2);
    else if (check_condition_type2_result == 2)legal_result = 0; //语法错误
    if (check_condition_type3_result == 1)legal_result = legal_test_simple(code_str_condition_type3);
    else if (check_condition_type3_result == 2)legal_result = 0; //语法错误
    
    if(legal_result==1){
        Serial.println("Check Finished: Legal Code!");
        //显示表情表示程序正确/播放开始运行程序/需要记录之前的表情
        //...
        //
        
        
        instant_stop=false;
        start_cypher=true;

        

        vTaskResume(Code_Process_Handle);
    }else if(legal_result==0){
        Serial.println("Check Finished: False Code!");
        //表情

        //声音

    }else if(legal_result==3){
        Serial.println("Check Finished: Empty Code!");
        //表情

        //声音
    }else if(legal_result==2){
        Serial.println("Check Finished: Oversize Code!");
        //表情

        //声音
    }



}










