#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVector>
#include <QDebug>

class parser : public QObject
{
    Q_OBJECT
public:
    explicit parser(QObject *parent = nullptr);


    void setWebPage(QString webPage_);

    QString getDate(int date_num = 0);
    QString getDatehResURL(int date_num = 0, int res = 5); //should go from 0 to 5, from lower to bigger res








    bool parse_web_page(QString webPage_ = "");
private:

    const QString date_line_trigger = "<h3 class=\"hdl-image-list-small bottom-10\">";
    const QString date_line_end_trigger = "</h3>";

    static const int available_res_num = 6;
    const QString available_res[available_res_num] = {"1024 x 768","1280 x 1024","1280 x 800","1366 x 768","1680 x 1050", "1920 x 1080"};

    QString web_page;


    static  const int num_of_dates_stored = 3;

    //these two go "paired"
    QString last_dates[num_of_dates_stored];
    QMap<QString, QString> date_links[num_of_dates_stored];

signals:

public slots:




};

#endif // PARSER_H
