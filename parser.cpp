#include "parser.hpp"

parser::parser(QObject *parent) : QObject(parent)
{
}

void parser::setWebPage(QString webPage_){
    web_page = webPage_;
}

QString parser::getDate(int date_num){
    return last_dates[date_num];
}

QString parser::getDatehResURL(int date_num, int res){ //should go from 0 to 5
    if(res<0 || res > 5){
        qCritical() << "Invalid resolution selected (QString parser::getLastMonthRes(int res))";
        return "";
    }


    return date_links[date_num].value(available_res[res]);

}


bool parser::parse_web_page(QString webPage_ /* = ""*/){
    if(!webPage_.isEmpty()){
        web_page = webPage_;
    }
    if(web_page.isEmpty()){
        return false;
    }

    QStringList list = web_page.split("\n", QString::SkipEmptyParts );

    int selected_date = -1;
    QString date_name = "";

    for (int i = 0; i < list.size(); ++i){
        QString line = list.at(i).toLocal8Bit().constData();







        if(line.contains(date_line_trigger, Qt::CaseInsensitive)){

            /* QStringList subList = line.split('"');
            for(QString str : subList){
                if(str.contains(".jpg")){
                    return true;//gota check it
                }*/

            selected_date++;
            if(selected_date == (num_of_dates_stored)){
                return true;
            }


            int pos_to_date_start = line.indexOf(date_line_trigger) + date_line_trigger.length();
            int date_length = line.indexOf(date_line_end_trigger) - pos_to_date_start;



            date_name = line.mid(pos_to_date_start, date_length);
            //qDebug() << date_name;
            last_dates[selected_date] = date_name;

        }

        if(selected_date !=-1){
            for(int x = 0; x < available_res_num; x++){
                if(line.contains(available_res[x], Qt::CaseInsensitive)){//we need to get here the links

                    //sample string: "\t\t\t\t\t\t\t<a href=\"/fileadmin/media/service/multimedia/wallpaper/Wallpaper_Mai_2019/1905_1024.jpg\" target=\"_blank\" class=\"block icon-link-small icon-image bg-grey-19\"><span class=\"block no-padding-12 link-black\">1024 x 768</span></a>"


                    QStringList subList = line.split('"');
                    for(QString str : subList){
                        if(str.contains(".jpg")){
                            //qDebug() <<"Download URL: "<< str;
                            date_links[selected_date].insert(available_res[x],str);

                            //qDebug()<< "MONTH:"<<last_dates[selected_date];
                            //qDebug()<< "RES:"<<available_res[x];
                            //qDebug()<< "URL:"<<date_links[selected_date].value(available_res[x]);
                        }
                    }
                }
            }
        }
    }

    if (selected_date == -1){//didnt found anything
        return false;
    }else{
        return true;
    }
}



