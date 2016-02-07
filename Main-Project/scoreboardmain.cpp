#include "scoreboardmain.h"
#include "ui_scoreboardmain.h"
#include <windows.h> //For Hotkey/Shortcut key
#include <fstream> //For Outputing to .txt
#include <ctime>
#include <QTimer>
#include <QString>
#include <string>
#include <QMessageBox>
#include <QFileDialog>
#include <sstream>
#include <QMediaMetaData>

using namespace std;

int Home_Score = 0, Away_Score = 0; //Team Score integer
int Period = 1; //Period integer | Default = 1
int minu = 0, seco = 0; //Minutes and Seconds integer
int mins = 0, secs = 0; //Used for To_input
int saved_speed = 0, last_speed = 0; //Stored speed integer
int speed = 1000; //Speed integer | Default = 1000 (1seconds) | Milliseconds = 1000(0.1seconds)
int Clock_button = 0; //Start/Stop button
int Normal_Speed = 1000, Millisecond_Speed = 93;
int PresetRadio = 0;

bool bol = false; //Open/Close for the server
bool many = false; //For milliseconds register
bool input_stop = false; //Stop getting the number from the input
bool getinput = false;
bool testplayer = false; //Test player button
bool to_switch; //On/Off for To_input
bool Milliseconds = false, Minute_Zero = false, Hotkey = false, Stopwatch_input = false; //On/Off for Milliseconds, Add zero to minute, Hotkey, Stopwatch
bool presetbool = false; //Preset timer
bool SecretIanButton = false; //Secret Ian Button

string minutes_zero = "", seconds_zero = ""; //For 9 <-> 0 | Example: 09 05
string Home_Name = "", Away_Name = ""; //Name for Home, Away
string clock_symbol = ":"; //Clock Symbol | Default = : | Milliseconds = .
string Window_Name = "Scoreboard+ 1.8.1Dev"; //Please Change this after a update!
QString Clock_text = "00:00"; //Clock Text

ofstream Home_Name_Output, Away_Name_Output, Home_Score_Output, Away_Score_Output, Period_Output, Clock_Output; //Ofstream for outputting to .txt

ScoreboardMain::ScoreboardMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScoreboardMain)
{
    ui->setupUi(this);
    di = new Dialog(this);
    di->show();
    ScoreboardMain::Opened(); //Declares/Resets Opened() DO NOT DELETE
}



ScoreboardMain::~ScoreboardMain()
{
    delete ui;
}

void ScoreboardMain::Opened() //Resets all
{
    writexml();
    Home_Name_Output.open(".\\Output\\Home_Name.txt");
    Away_Name_Output.open(".\\Output\\Away_Name.txt");
    Home_Score_Output.open(".\\Output\\Home_Score.txt");
    Away_Score_Output.open(".\\Output\\Away_Score.txt");
    Period_Output.open(".\\Output\\Period.txt");
    Clock_Output.open(".\\Output\\Clock.txt");

    Home_Name_Output << "";
    Away_Name_Output << "";
    Home_Score_Output << Home_Score;
    Away_Score_Output << Away_Score;
    Period_Output << Period;
    Clock_Output << "00:00";

    Home_Name_Output.close();
    Away_Name_Output.close();
    Home_Score_Output.close();
    Away_Score_Output.close();
    Period_Output.close();
    Clock_Output.close();

    ui->PeriodUP_Button->setText((QString::fromWCharArray(L"\u25B3")));
    QFont font0 = ui->PeriodUP_Button->font();
    font0.setPointSize(20);
    ui->PeriodUP_Button->setFont(font0);

    ui->PeriodDOWN_Button->setText((QString::fromWCharArray(L"\u25BD")));
    QFont font1 = ui->PeriodDOWN_Button->font();
    font1.setPointSize(20);
    ui->PeriodDOWN_Button->setFont(font1);

    ui->HomeUP_Button->setText((QString::fromWCharArray(L"\u25B3")));
    QFont font2 = ui->HomeUP_Button->font();
    font2.setPointSize(20);
    ui->HomeUP_Button->setFont(font2);

    ui->HomeDOWN_Button->setText((QString::fromWCharArray(L"\u25BD")));
    QFont font3 = ui->HomeDOWN_Button->font();
    font3.setPointSize(20);
    ui->HomeDOWN_Button->setFont(font3);

    ui->AwayUP_Button->setText((QString::fromWCharArray(L"\u25B3")));
    QFont font4 = ui->AwayUP_Button->font();
    font4.setPointSize(20);
    ui->AwayUP_Button->setFont(font4);

    ui->AwayDOWN_Button->setText((QString::fromWCharArray(L"\u25BD")));
    QFont font5 = ui->AwayDOWN_Button->font();
    font5.setPointSize(20);
    ui->AwayDOWN_Button->setFont(font5);

    this->setFixedSize(this->size());
    this->setWindowTitle(Window_Name.c_str());
    ui->StopMusic_Button->setVisible(false);
    ui->Warning_Label->setVisible(false);

    //Makes new QNetworkAccessMangager and parents to this
    manager = new QNetworkAccessManager(this);
    //Connect to replyFinished QnetworkReply
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    //Get From the URL
    manager->get(QNetworkRequest(QUrl("https://obsproject.com/forum/resources/scoreboard-windows-mac.150/")));
}

void ScoreboardMain::replyFinished(QNetworkReply *reply)
{
    string getstring = "";
    size_t str;
    int a = 0;
    bool updateyes = false;
    //Check if there is a error
    if(reply->error())
    {
        ui->Testing->setText("Error: 0001, Not able to check for updates!");
    }else{
    ofstream textout;
    textout.open(".\\Update\\out.txt");
    //Read all from the url and writes to a file
    textout << reply->readAll().toStdString();
    textout.close();
    ifstream textin;
    textin.open(".\\Update\\out.txt");
    if(textin.is_open())
    {
        while(getline(textin, getstring))
        {
            ui->Testing->setStyleSheet("QLabel{color: rgb(0, 0, 0);}");
        str = getstring.find("Version_Info:_1_8_1_Dev");
        if(str!=string::npos)
        {
            ui->Testing->setText("Status: No Update");
            return;
            break;
            updateyes = true;
        }
        a++;
        }
        if(a > getstring.size() && updateyes == false)
        {
           ui->Testing->setStyleSheet("QLabel{color: rgb(255, 0, 0);}");
           ui->Testing->setText("Status: New Update Found! Please Update This Program!");
           WarningBox();
        }
    }
    textin.close();
    }
    reply->deleteLater();
}
int stopreply = 0;
void ScoreboardMain::WarningBox()
{
    if(stopreply == 0)
    {
       QMessageBox::warning(this,"New Update Found", "<a href=https://obsproject.com/forum/resources/scoreboard-windows-mac.150>Update Download</a>");
       stopreply = 1;
    }else{
    }
}

void ScoreboardMain::Changed() //Changed Score,etc
{
    if(ui->checkBox->isChecked())
    {
        writexml();
    }else{
        Home_Score_Output.open(".\\Output\\Home_Score.txt");
        Away_Score_Output.open(".\\Output\\Away_Score.txt");
        Period_Output.open(".\\Output\\Period.txt");

        Home_Score_Output << Home_Score;
        Away_Score_Output << Away_Score;
        Period_Output << Period;

        Home_Score_Output.close();
        Away_Score_Output.close();
        Period_Output.close();
    }
    di->Home_Score(Home_Score);
    di->Away_Score(Away_Score);
    di->period(Period);

    ui->HomeScore_Label->setText(QString::number(Home_Score));
    ui->AwayScore_Label->setText(QString::number(Away_Score));
    ui->Period_Label->setText(QString::number(Period));
}

void ScoreboardMain::on_HomeUP_Button_clicked() //Home_Up Button
{
    Home_Score++;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Home_Score:" + QString::number(Home_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_HomeUP2_Button_clicked() //Home_+2 Button
{
    Home_Score+=2;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Home_Score:" + QString::number(Home_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_HomeDOWN_Button_clicked() //Home_Down Button
{
    Home_Score--;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Home_Score:" + QString::number(Home_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_AwayUP_Button_clicked() //Away_Up Button
{
    Away_Score++;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Away_Score:" + QString::number(Away_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_AwayUp2_Button_clicked() //Away_+2 Button
{
    Away_Score+=2;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Away_Score:" + QString::number(Away_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_AwayDOWN_Button_clicked() //Away_Down Button
{
    Away_Score--;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Away_Score:" + QString::number(Away_Score) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_PeriodUP_Button_clicked() //Period_Up Button
{
    Period++;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Period:" + QString::number(Period) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_PeriodDOWN_Button_clicked() //Period_Down Button
{
    Period--;
    ScoreboardMain::Changed();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Period:" + QString::number(Period) + "\n").toUtf8());
    }else{
        return;
    }
}

void ScoreboardMain::on_Update_Team_Button_clicked() //Update Team Name Button
{
    QString HomeN = ui->HomeName_Input->text(), AwayN = ui->AwayName_Input->text();
    Home_Name = HomeN.toUtf8().constData(), Away_Name = AwayN.toUtf8().constData();
    if(ui->checkBox->isChecked())
    {
        writexml();
    }else{
    Home_Name_Output.open(".\\Output\\Home_Name.txt");
    Away_Name_Output.open(".\\Output\\Away_Name.txt");
    Home_Name_Output << Home_Name;
    Away_Name_Output << Away_Name;
    Home_Name_Output.close();
    Away_Name_Output.close();
    }
    di->Home_Name(QString::fromStdString(Home_Name));
    di->Away_Name(QString::fromStdString(Away_Name));
}

void ScoreboardMain::on_Reset_Score_Button_clicked() //Reset Score Button
{
    Home_Score = 0;
    Away_Score = 0;
    ScoreboardMain::Changed();
}

void ScoreboardMain::on_Swap_Button_clicked() //Swap Button
{
    int scoreh1 = Home_Score;
    string Homen1 = Home_Name;
    Home_Score = Away_Score;
    Away_Score = scoreh1;
    Home_Name = Away_Name;
    Away_Name = Homen1;
    ui->HomeScore_Label->setText(QString::number(Home_Score));
    ui->AwayScore_Label->setText(QString::number(Away_Score));
    ui->HomeName_Input->setText(QString::fromStdString(Home_Name));
    ui->AwayName_Input->setText(QString::fromStdString(Away_Name));
    di->Home_Name(QString::fromStdString(Home_Name));
    di->Away_Name(QString::fromStdString(Away_Name));
    di->Home_Score(Home_Score);
    di->Away_Score(Away_Score);
    if(ui->checkBox->isChecked())
    {
        writexml();
    }else{
    Away_Score_Output.open(".\\Output\\Away_Score.txt");
    Away_Score_Output << Away_Score;
    Away_Score_Output.close();
    Home_Score_Output.open(".\\Output\\Home_Score.txt");
    Home_Score_Output << Home_Score;
    Home_Score_Output.close();
    Home_Name_Output.open(".\\Output\\Home_Name.txt");
    Away_Name_Output.open(".\\Output\\Away_Name.txt");
    Home_Name_Output << Home_Name;
    Away_Name_Output << Away_Name;
    Home_Name_Output.close();
    Away_Name_Output.close();
    }
}


void ScoreboardMain::on_Alway_on_top_Checkbox_clicked(bool alwayson) //Always on top Checkbox
{
    Qt::WindowFlags flags = windowFlags();
    if (alwayson)
    {
        flags ^= Qt::WindowStaysOnBottomHint;
        flags |= Qt::WindowStaysOnTopHint;
    }
    else
    {
        flags ^= Qt::WindowStaysOnTopHint;
        flags |= Qt::WindowStaysOnBottomHint;
    }
    setWindowFlags(flags);
    show();
}

void ScoreboardMain::on_Start_Button_clicked() //Start button
{
    Clock_button++;
    if(minu > 0 && many == false && Stopwatch_input == false){
    speed = Normal_Speed;
    clock_symbol = ":";
    }
    /*if(countu == true && milli == false)
    {
        if(seco == 0)
        {
            seco = -1;
        }else{
    seco--;
    }
        }*/
    if(minu == 0 && Milliseconds == true && Stopwatch_input == false)
    {
        many = true;
        speed = Millisecond_Speed;
        minu = seco;
        minu--;
        seco = 10;
        seconds_zero = "";
        clock_symbol = ".";
    }else if(minu == 0 && Stopwatch_input == false){
    speed = Normal_Speed;
    clock_symbol = ":";
    many = false;
    }
    if(many == false && Stopwatch_input == true && Milliseconds == true)
    {
    many = false;
    speed = Millisecond_Speed;
    minu = seco;
    clock_symbol = ".";
    }
    if(minu == 0 && many == false && Stopwatch_input == true && Milliseconds == false)
    {
    many = true;
    speed = Normal_Speed;
    clock_symbol = ":";
    if(seco < 9 && many == true)
    {
        seconds_zero = "0";
    }
    if(seco > 9)
    {
    seconds_zero = "";
    }
    }
    if(Clock_button == 2 && Stopwatch_input == false)
    {
        input_stop = false;
        clock->stop();
        ui->Start_Button->setText("Start");
    }
    if(Clock_button == 2 && Stopwatch_input == true)
    {
       input_stop = false;
       clock->stop();
       ui->Start_Button->setText("Start");
    }
    if(Clock_button == 3 && Stopwatch_input == false)
    {
        if(speed == Normal_Speed)
        {
            speed = Normal_Speed;
        }else if(speed == Millisecond_Speed)
        {
            speed = Millisecond_Speed;
        }
        input_stop = true;
        Clock_button = 1;
    }
    if(Clock_button == 3 && Stopwatch_input == true)
    {
     Clock_button = 1;
     if(saved_speed == Normal_Speed)
     {
         speed = Normal_Speed;
     }else if(saved_speed == Millisecond_Speed)
     {
         speed = Millisecond_Speed;
     }
    }
    if(Clock_button == 1 && Stopwatch_input == false)
    {
    input_stop = true;
    ui->Start_Button->setText("Stop");
    ScoreboardMain::Timer_Control();
    }
    if(Clock_button == 1 && Stopwatch_input == true)
    {
    input_stop = true;
    ui->Start_Button->setText("Stop");
    ScoreboardMain::Stopwatch_Control();
    }
}

void ScoreboardMain::Stopwatch_Control() //Stopwatch Control
{
    clock = new QTimer(this);
    connect(clock,SIGNAL(timeout()),this,SLOT(Stopwatch()));
    if(seco < 9 && Milliseconds == false)
    {
        seconds_zero = "0";
    }
    if(seco > 9)
    {
    seconds_zero = "";
    }
       // seco++;
    if(minu > 1 && Milliseconds == true)
    {
        speed = Normal_Speed;
        clock_symbol = ":";
        many = true;
        if(seco < 9)
        {
            seconds_zero = "0";
        }
        if(seco > 9)
        {
        seconds_zero = "";
        }
    }
    clock->start(speed);
}

void ScoreboardMain::Timer_Control() //Timer Control
{
    clock = new QTimer(this);
    connect(clock,SIGNAL(timeout()),this,SLOT(Timer()));
    if(seco < 9 && many == false)
    {
        seconds_zero = "0";
    }
    if(seco > 9)
    {
    seconds_zero = "";
    }
    if(minu < 9 && many == false && Minute_Zero == true)
    {
        minutes_zero = "0";
    }
    if(minu > 9 && Minute_Zero == true)
    {
    minutes_zero = "";
    }
    if(minu == -1)
    {
        minu++;
    }
    //seco++; //the problem need to be fixed!
    clock->start(speed);
}

void ScoreboardMain::Stopwatch() //Stopwatch
{
    if(minu == 59 && seco == 9 && Milliseconds == true)
    {
        speed = Normal_Speed;
        minu = 1;
        many = true;
        seco = 0;
        Milliseconds = false;
        seconds_zero = "0";
        clock_symbol = ":";
        clock->start(speed);
    }
    if(ui->Minutes_Input->value() == 0 && many == true)
    {
        if(seco == 0)
        {
            seconds_zero = "0";

        many = true;
        clock_symbol = ":";
        speed = Normal_Speed;
    }
    }
    if(minu < 10 && Minute_Zero == true)
    {
        minutes_zero = "0";
    }else
    {
        minutes_zero = "";
    }
    ui->Clock_Label->setText("  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco));
    if(ui->checkBox->isChecked())
    {
        Clock_text = QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco);
        writexml();
    }
    Clock_Output.open(".\\Output\\Clock.txt");
    Clock_Output << minutes_zero << minu << clock_symbol << seconds_zero << seco;
    Clock_Output.close();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Clock:  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco)).toUtf8());
    }
    if(getinput == true)
    {
        ui->Minutes_Input->setValue(minu);
        ui->Seconds_Input->setValue(seco);
    }
    if(seco < 9 && Milliseconds == false)
    {
        seconds_zero = "0";
    }else if(seco > 8 && Milliseconds == false){
        seconds_zero = "";
    }
    if(minu == mins && seco == secs && to_switch == true)
    {
        ScoreboardMain::on_Start_Button_clicked();
        if(!ui->BrowseFile_Input->text().isEmpty())
        {
        player->play();
        ui->StopMusic_Button->setVisible(true);
        ui->TestSound_Button->setText("Stop Sound");
        }
    }
    if(Milliseconds == true && many == false && seco == 9)
    {
    seco = 0;
    minu++;
    seconds_zero = "";
    }
    if(seco == 59)
    {
        seco = -1;
        seconds_zero = "0";
        clock_symbol = ":";
        minu++;
    }
    seco++;
}

void ScoreboardMain::Timer() //Timer
{
    //seco--;
    if(seco > 11)
    {
    seconds_zero = "";
    }
 ui->Clock_Label->setText("  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco));
 if(ui->checkBox->isChecked())
 {
     Clock_text = QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco);
     writexml();
 }
 Clock_Output.open(".\\Output\\Clock.txt");
 Clock_Output << minutes_zero << minu << clock_symbol << seconds_zero << seco;
 Clock_Output.close();
 if(ui->pushButton->text() == "Disable")
 {
     client->write(QString("Clock:  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco)).toUtf8());
 }
 if(getinput == true)
 {
     ui->Minutes_Input->setValue(minu);
     ui->Seconds_Input->setValue(seco);

 }
 if(minu == 1 && Milliseconds == true && seco == 0 && many == false)
 {
     many = true;
     seco = 10;
     minu = 59;
     speed = Millisecond_Speed;
     seconds_zero = "";
     clock_symbol = ".";
     clock->start(speed);
 }
 if(seco == 0 && many == false)
 {
   seco = 60;
   minu--;
 }
 if(minu < 10 && many == 0 && Minute_Zero == true)
 {
     minutes_zero = "0";
 }else
 {
     minutes_zero = "";
 }
 if(minu == mins && seco == secs && to_switch == true)
 {
     ScoreboardMain::on_Start_Button_clicked();
     if(!ui->BrowseFile_Input->text().isEmpty())
     {
     player->play();
     ui->StopMusic_Button->setVisible(true);
     ui->TestSound_Button->setText("Stop Sound");
     }
     }
 if(seco < 11 && many == false)
 {
     seconds_zero = "0";
 }
 if(minu < 10 && many == 0 && Minute_Zero == true)
 {
     minutes_zero = "0";
 }else
 {
     minutes_zero = "";
 }
 if(seco == 0 && many == true)
 {
     minu--;
     seco = 10;
 }
 seco--;
 if(minu == -1 && seco == 59 && Milliseconds == false)
 {
     clock->stop();
     ui->Testing->setText("Error Output:");
     ui->Clock_Label->setText("  00:00");
     ui->Start_Button->setText("Start");
     Clock_button = 0;
     minu = 0;
     seco = 0;
     input_stop = false;
     many = 0;
     ui->Seconds_Input->setValue(0);
     ui->Minutes_Input->setValue(0);
     speed = Normal_Speed;
     if(!ui->BrowseFile_Input->text().isEmpty())
     {
     player->play();
     ui->StopMusic_Button->setVisible(true);
     ui->TestSound_Button->setText("Stop Sound");
     }
     clock->stop();
 }
 if(minu == 0 && seco == 0 && Milliseconds == true)
 {
     clock->stop();
     ui->Testing->setText("Error Output:");
     ui->Clock_Label->setText("  00:00");
     ui->Start_Button->setText("Start");
     Clock_button = 0;
     minu = 0;
     seco = 0;
     input_stop = false;
     many = 0;
     ui->Seconds_Input->setValue(0);
     ui->Minutes_Input->setValue(0);
     speed = Normal_Speed;
     if(!ui->BrowseFile_Input->text().isEmpty())
     {
     player->play();
     ui->StopMusic_Button->setVisible(true);
     ui->TestSound_Button->setText("Stop Sound");
     }
     clock->stop();
 }
}

void ScoreboardMain::on_Reset_Button_clicked() //Reset Clock button
{
    if(Clock_button == 1)
    {
        ui->Testing->setText("Error Output: Please stop your timer");
    }else{
    Clock_button = 0,many = 0;
    if(presetbool == true && PresetRadio == 1 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  5:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(5);
        clock_symbol = ":";
        minu = 5;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "5:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "5:00";
            writexml();
        }
        ui->Clock_Label->setText("  5:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  5:00").toUtf8());
        }
    }else if(presetbool == true && PresetRadio == 2 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  10:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(10);
        clock_symbol = ":";
        minu = 10;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "10:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "10:00";
            writexml();
        }
        ui->Clock_Label->setText("  10:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  10:00").toUtf8());
        }
    }else if(presetbool == true && PresetRadio == 3 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  15:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(15);
        clock_symbol = ":";
        minu = 15;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "15:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "15:00";
            writexml();
        }
        ui->Clock_Label->setText("  15:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  15:00").toUtf8());
        }
    }else if(presetbool == true && PresetRadio == 4 && Stopwatch_input == false)
    {
        ui->Clock_Label->setText("  20:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(20);
        minu = 20;
        seco = 0;
        clock_symbol = ":";
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "20:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "20:00";
            writexml();
        }
        ui->Clock_Label->setText("  20:00");
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  20:00").toUtf8());
        }
    }else{
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        ui->Clock_Label->setText("  00:00");
        ui->Seconds_Input->setValue(0);
        ui->Minutes_Input->setValue(0);
        minu = 0;
        seco = 0;
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        ui->Clock_Label->setText("  00:00");
        clock_symbol = ":";
        if(ui->pushButton->text() == "Disable")
        {
        client->write(QString("Clock:  00:00").toUtf8());
        }
    }
    many = false;
    ui->Testing->setText("Error Output:");
}
}

void ScoreboardMain::on_To_Checkbox_clicked(bool to)
{
    to_switch = to;
}

void ScoreboardMain::on_Minutes_Input_valueChanged(int arg0)
{
    if(input_stop == false)
    {
    minu = arg0;
    ifclockinputchanged();
    }
    }

void ScoreboardMain::on_Seconds_Input_valueChanged(int arg1)
{
    if(input_stop == false)
    {
    seco = arg1;
     ifclockinputchanged();
    }
}

void ScoreboardMain::on_Minutes_To_In_valueChanged(int arg2)
{
    mins = arg2;
}

void ScoreboardMain::on_Seconds_To_Input_valueChanged(int arg3)
{
    secs = arg3;
}

void ScoreboardMain::on_Addzero_Checkbox_clicked(bool add)
{
    Minute_Zero = add;
}

void ScoreboardMain::on_CurrentTime_Checkbox_clicked(bool current)
{
    if(current == true)
    {
        currenttime = new QTimer(this);
        connect(currenttime, SIGNAL(timeout()),this, SLOT(timeclock()));
        currenttime->start(1000);
    }else{
        currenttime->stop();
        ui->Clock_Label->setText("  00:00");
        if(ui->checkBox->isChecked())
        {
            Clock_text = "00:00";
            writexml();
        }
        Clock_Output.open(".\\Output\\Clock.txt");
        Clock_Output << "00:00";
        Clock_Output.close();
        seconds_zero = "";
        minutes_zero = "";
}
}

void ScoreboardMain::timeclock()
{
    time_t current = time(0);
    tm *local = localtime(&current);
    if(local->tm_sec < 10)
    {
        seconds_zero = "0";
    }else{
        seconds_zero = "";
    }
    if(local->tm_min < 10)
    {
        minutes_zero = "0";
    }else{
        minutes_zero = "";
    }
    ui->Clock_Label->clear();
    ui->Clock_Label->setText(QString::number(local->tm_hour) + ":" + QString::fromStdString(minutes_zero) + QString::number(local->tm_min) + ":" + QString::fromStdString(seconds_zero) + QString::number(local->tm_sec));
    if(ui->checkBox->isChecked())
    {
        Clock_text = QString::number(local->tm_hour) + ":" + QString::fromStdString(minutes_zero) + QString::number(local->tm_min) + ":" + QString::fromStdString(seconds_zero) + QString::number(local->tm_sec);
    }
    Clock_Output.open(".\\Output\\Clock.txt");
    Clock_Output << local->tm_hour << ":" << minutes_zero << local->tm_min << ":" << seconds_zero << local->tm_sec;
    Clock_Output.close();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Clock:" + QString::number(local->tm_hour) + ":" + QString::fromStdString(minutes_zero) + QString::number(local->tm_min) + ":" + QString::fromStdString(seconds_zero) + QString::number(local->tm_sec)).toUtf8());
    }
}

void ScoreboardMain::ifclockinputchanged()
{
    if(seco < 10 && many == false)
    {
        seconds_zero = "0";
    }else{
        seconds_zero = "";
    }
    ui->Clock_Label->setText("  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco));
    if(ui->checkBox->isChecked())
    {
        Clock_text = QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco);
        writexml();
    }
    Clock_Output.open(".\\Output\\Clock.txt");
    Clock_Output << minutes_zero << minu << clock_symbol << seconds_zero << seco;
    Clock_Output.close();
    if(ui->pushButton->text() == "Disable")
    {
        client->write(QString("Clock:  " + QString::fromStdString(minutes_zero) + QString::number(minu) + QString::fromStdString(clock_symbol) + QString::fromStdString(seconds_zero) + QString::number(seco)).toUtf8());
    }
}

void ScoreboardMain::on_Stopwatch_Checkbox_clicked(bool stop)
{
    Stopwatch_input = stop;
}

void ScoreboardMain::on_Milliseconds_Checkbox_clicked(bool milli)
{
    Milliseconds = milli;
}

void ScoreboardMain::on_Countdown_Checkbox_clicked(bool countdown)
{
    getinput = countdown;
}

void ScoreboardMain::on_Hotkey_Checkbox_clicked(bool Hotkey1)
{
    const int speed2 = 300;
        Hotkey = Hotkey1;
        if(Hotkey == false)
        {
            hotkey->stop();
        }
    hotkey = new QTimer(this);
    connect(hotkey,SIGNAL(timeout()),this,SLOT(hotkeys()));
    hotkey->start(speed2);
}

void ScoreboardMain::hotkeys()
{
    if(GetAsyncKeyState(VK_LCONTROL) && Hotkey == true)
    {
        Home_Score++;
        ScoreboardMain::Changed();
    }
    if(GetAsyncKeyState(VK_RCONTROL) && Hotkey == true)
    {
        Away_Score++;
        ScoreboardMain::Changed();
    }
    if(GetAsyncKeyState(VK_LMENU) && Hotkey == true)
    {
        ScoreboardMain::on_Start_Button_clicked();
    }
}

void ScoreboardMain::on_About_ActionBar_triggered()
{
    QMessageBox about;
    about.setText("About");
    about.setInformativeText("                                                                                                                  ");
    about.setInformativeText("This Program is made by: ha0511kr \n Idea by: XxRocketShotxX \n Adding Soon: \n "
                             "-Able to set hotkeys \n -Adding Icon to the program \n -Fix Glitches and Bugs from the Timer or Scoreboard \n"
                             " -And all of your suggetions \n"
                             "\nIf you have any questions or suggetions please contact me in \n"
                             "Email: ha0511kr.com@gmail.com \n"
                             "OBS fourm page disscusion \n"
                             "\nThank you for downloading and enjoy the scoreboard and timer");
    about.exec();
}

void ScoreboardMain::on_Hotkey_ActionBar_triggered()
{
    QMessageBox hot;
    hot.setText("Hotkey Help");
    hot.setInformativeText("Home score = Left Control \nAway score = Right Control \nTimer = Left Alt \nConfiguring Hotkey will be added in later versions");
    hot.exec();
}

void ScoreboardMain::on_ChangeLog_ActionBar_triggered()
{
    QMessageBox Changelog;
    Changelog.setText("Changelog:");
    Changelog.setInformativeText("1.3Dev: \n"
                           "-Added Hotkeys\n"
                           "-Able to change team names\n"
                           "-Fixed Glitches and bugs\n"
                           "1.7Dev: \n"
                           "-Added hotkey for Timer/Stopwatch \n"
                           "-Added Stopwatch\n"
                           "-Added ToolBar\n"
                           "-Able to use hotkey anytime without timer/stopwatch being on\n"
                           "-Fixed Glitches and bugs\n"
                           "1.7.5Dev: \n"
                           "-Added Millisec to stopwatch\n"
                           "-Fixed bug with Timer and Stopwatch\n"
                           "-Added Feature from Jules_B: Stopwatch stopping in predefined time\n"
                           "1.7.9Dev: \n"
                           "-Added Swap Button\n"
                           "-Added +2 for Basketball\n"
                           "-Fixed Bug with Stopwatch/Timer\n"
                           "1.7.10Dev: \n"
                           "Fix Bugs and Glitches \n"
                           "Added Countdown Input Box for easy use \n"
                           "Added Reset Score button \n"
                           "Better Look for setting tab \n"
                           "Changed button size and name ex: Update Teamname -> Update TeamN \n"
                           "Added Console Tab Coming soon. \n"
                           "-------------------------------------------------------------------------\n"
                           "Coming Soon:\n"
                           "Console Tab\n"
                           "And More... Please leave a suggestions in the comments");
    Changelog.exec();
}

void ScoreboardMain::on_Usehotkey_Help_clicked()
{
    ScoreboardMain::on_Hotkey_ActionBar_triggered();
}

void ScoreboardMain::on_pushButton_clicked()
{
    if(bol == false)
    {
    //Connects to Acceptconnection if they are connected
    connect(&server, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    //Server is listening to the IP and port
    server.listen(QHostAddress::Any, 1234);
    ui->Status->setStyleSheet("QLabel {color: rgb(225, 227, 0);}");
    ui->Status->setText("Status: Listening");
    ui->pushButton->setText("Disable Listen");
    QMessageBox::warning(this, tr("Warning!"), tr("Please know that remote feature is in beta.\nIt could crash the program."), QMessageBox::Ok);
    bol = true;
    }else if(bol == true){
        server.close();
        server.disconnect();
        if(!client->isOpen())
        {
        client->close();
        }
        ui->pushButton->setText("Enable");
        ui->Status->setStyleSheet("QLabel {color: rgb(255, 0, 0);}");
        ui->Status->setText("Status: Not Connected");
        bol = false;
    }
}

void ScoreboardMain::startRead()
{
    //Loops if server can read string from the cliehnt
    while(client->canReadLine())
    {
    //Reads from the client and put it to QString line
    QString line = QString::fromUtf8(client->readLine()).trimmed();

    if(line == "Home_Up")
    {
        Home_Score++;
        Changed();
        //Writes/Sends string/Score to the client
        client->write("Home_Score:" + QByteArray::number(Home_Score));
    }
    if(line == "Away_Up")
    {
        Away_Score++;
        Changed();
        client->write("Away_Score:" + QByteArray::number(Away_Score));
    }
    if(line == "Home_Down")
    {
        Home_Score--;
        Changed();
        client->write("Home_Score:" + QByteArray::number(Home_Score));
    }
    if(line == "Away_Down")
    {
        Away_Score--;
        Changed();
        client->write("Away_Score:" + QByteArray::number(Away_Score));
    }
    if(line == "Period_Up")
    {
        Period++;
        Changed();
        client->write("Period:" + QByteArray::number(Period));
    }
    if(line == "Period_Down")
    {
        Period--;
        Changed();
        client->write("Period:" + QByteArray::number(Period));
    }
    if(line == "Clock_Button")
    {
        on_Start_Button_clicked();
        client->write("Clock_Button:" + QByteArray::fromStdString(ui->Start_Button->text().toUtf8().constData()));
    }
    if(line.contains("Min:"))
    {
        line.remove("Min:");
        int minui = line.toInt();
        ui->Minutes_Input->setValue(minui);
        minu = minui;
    }
    if(line.contains("Sec:"))
    {
        line.remove("Sec:");
        int secoi = line.toInt();
        ui->Seconds_Input->setValue(secoi);
        seco = secoi;
    }
    if(line.contains("Home_Name:"))
    {
        line.remove("Home_Name:");
        ui->HomeName_Input->setText(line);
        on_Update_Team_Button_clicked();
    }
    if(line.contains("Away_Name:"))
    {
        line.remove("Away_Name:");
        ui->AwayName_Input->setText(line);
        on_Update_Team_Button_clicked();
    }
    if(line == "Reset")
    {
        on_Reset_Button_clicked();
    }
    if(line == "Stop_Watch")
    {
        Stopwatch_input = true;
    }
    if(line == "Stop_Watch_Stop")
    {
        Stopwatch_input = false;
    }
    if(line == "Current_Time")
    {
        on_CurrentTime_Checkbox_clicked(true);
    }
    if(line == "Current_Time_Stop")
    {
        on_CurrentTime_Checkbox_clicked(false);
    }
    if(line == "Reset_Score")
    {
        Away_Score = 0;
        Home_Score = 0;
        Changed();
    }
    ui->Status->setText("Client says: " + line);
    }
}


void ScoreboardMain::acceptConnection()
{
    //Connecting to the next pending connection
    client = server.nextPendingConnection();
    //Set status to Connected
    ui->Status->setStyleSheet("QLabel {color: rgb(0, 220, 0);}");
    ui->Status->setText("Status: Connected! From: " + client->peerAddress().toString() + ":" + QString::number(client->peerPort()));
    client->write("Connected to Scoreboard+ \n");
    ui->pushButton->setText("Disable");
    //Connect to the startread to read from the client
    connect(client, SIGNAL(readyRead()),this,SLOT(startRead()));
}

void ScoreboardMain::on_actionSickmind33_triggered()
{
    QMessageBox Sick;
    Sick.setText("About: Sickmind33");
    Sick.setInformativeText("Thank you to Sickmind33 for creating an awesome icon!\nPress Ok to continue");
    Sick.exec();
}

void ScoreboardMain::on_PlaySound_Checkbox_clicked(bool checked10)
{
    if(checked10 == true)
    {
        ui->Browsefile_Button->setEnabled(true);
        ui->BrowseFile_Input->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->TestSound_Button->setEnabled(true);
        ui->SoundSpeed_Slider->setEnabled(true);
    }
    if(ui->TestSound_Button->text() == "Stop Sound")
    {
        ui->PlaySound_Checkbox->setChecked(true);
        ui->Warning_Label->setVisible(true);
    }else{
    if(checked10 == false)
    {
        ui->Warning_Label->setVisible(false);
         ui->TestSound_Button->setDisabled(true);
        ui->horizontalSlider->setDisabled(true);
        ui->Browsefile_Button->setDisabled(true);
        ui->BrowseFile_Input->setDisabled(true);
        ui->BrowseFile_Input->setText("");
        ui->SoundSpeed_Slider->setDisabled(true);
    }
    }
}

void ScoreboardMain::on_Browsefile_Button_clicked()
{
    QString Filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "All files (*.*);; mp3 File (*.mp3)");
    ui->BrowseFile_Input->setText(Filename);
    //player->setMedia(QUrl::fromLocalFile(Filename));
}

void ScoreboardMain::on_BrowseFile_Input_textChanged(const QString &arg1)
{
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(arg1));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(putduration()));
}

void ScoreboardMain::putduration()
{
    ui->Testing->setText(player->metaData(QMediaMetaData::Title).toString());
}

void ScoreboardMain::on_StopMusic_Button_clicked()
{
    player->stop();
    ui->Testing->setText("Error:");
    player->setPosition(0);
    ui->StopMusic_Button->setVisible(false);
    ui->TestSound_Button->setText("Test Sound");
}

void ScoreboardMain::on_horizontalSlider_sliderMoved(int position)
{
    player->setVolume(position);
}



void ScoreboardMain::on_TestSound_Button_clicked()
{
    if(testplayer == false && !(ui->BrowseFile_Input->text().isEmpty()))
    {
        player->play();
        testplayer = true;
        ui->TestSound_Button->setText("Stop Sound");
    }else if(testplayer == true && !(ui->BrowseFile_Input->text().isEmpty())){
        player->stop();
        ui->Testing->setText("Error:");
        testplayer = false;
        ui->TestSound_Button->setText("Test Sound");
        ui->StopMusic_Button->setVisible(false);
    }
}

bool truefalse = false;

void ScoreboardMain::on_Speed_CheckBox_clicked(bool checked11)
{
    if(checked11 == false)
    {
        Normal_Speed = 1000;
    }else if(checked11 == true){
        truefalse = true;
    }
}

void ScoreboardMain::on_SpeedHelp_Button_clicked()
{
    QMessageBox::warning(this,"Speed Help","Use this when your game timer/stopwatch is faster then this program\nDefault: 1000 = 1 sec");
}

void ScoreboardMain::on_Speed_Input_valueChanged(int arg13)
{
    if(truefalse == true)
    {
        Normal_Speed = arg13;
    }else if(truefalse == false){
        Normal_Speed = 1000;
    }
}
void ScoreboardMain::on_TimerPreset_Checkbox_clicked(bool checked15)
{
    if(checked15 == true)
    {
        presetbool = true;
        if(ui->FiveP_Radio->isChecked())
        {
            PresetRadio = 1;
        }
        if(ui->TenP_Radio->isChecked())
        {
            PresetRadio = 2;
        }
        if(ui->FifteenP_Radio->isChecked())
        {
            PresetRadio = 3;
        }
        if(ui->TwentyP_Radio->isChecked())
        {
            PresetRadio = 4;
        }
    }else{
        PresetRadio = 0;
        presetbool = false;
    }
}

void ScoreboardMain::on_FiveP_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 1;
    }
}

void ScoreboardMain::on_TenP_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 2;
    }
}

void ScoreboardMain::on_FifteenP_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 3;
    }
}

void ScoreboardMain::on_TwentyP_Radio_clicked()
{
    if(presetbool == true)
    {
    PresetRadio = 4;
    }
}

void ScoreboardMain::on_actionXxRocketshotxX_triggered()
{
    QMessageBox Xx;
    Xx.setText("About: XxRocketshotxX");
    Xx.setInformativeText("Thank you an idea of Scoreboard+!");
    Xx.exec();
}

void ScoreboardMain::xml()
{
    //Creates QFile to the directory
    QFile file(".//Output//Xml.xml");

    //Check if file is writeable
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->Testing->setText("xml Could not be created");
    }else{
    ScoreboardMain::writexml();
    }

}
int a = 0;

void ScoreboardMain::writexml()
{

    a++;
    //Creates a new xml file
    QFile file(".//Output//Xml.xml");
    //Make sure file is only writeonly
    file.open(QIODevice::WriteOnly);
    //Set QXmlStreamWriter to file path to &file
    QXmlStreamWriter xmlWriter(&file);
    //Auto Formatting for xml
    xmlWriter.setAutoFormatting(true);
    //Start document root
    xmlWriter.writeStartDocument();
    if(SecretIanButton == false)
    {
    xmlWriter.writeStartElement("items");
    xmlWriter.writeTextElement("timestamp",QString::number(a));
    xmlWriter.writeTextElement("HomeScore", QString::number(Home_Score));
    xmlWriter.writeTextElement("AwayScore", QString::number(Away_Score));
    xmlWriter.writeTextElement("HomeName", QString::fromStdString(Home_Name));
    xmlWriter.writeTextElement("AwayName", QString::fromStdString(Away_Name));
    xmlWriter.writeTextElement("Period", QString::number(Period));
    xmlWriter.writeTextElement("Clock", Clock_text);
    xmlWriter.writeEndElement();
    }else if(SecretIanButton == true){
        xmlWriter.writeStartElement("items");
        xmlWriter.writeTextElement("timestamp",QString::number(a));
        xmlWriter.writeTextElement("pScore1", QString::number(Home_Score));
        xmlWriter.writeTextElement("pScore2", QString::number(Away_Score));
        xmlWriter.writeTextElement("pName1", QString::fromStdString(Home_Name));
        xmlWriter.writeTextElement("pName2", QString::fromStdString(Away_Name));
        xmlWriter.writeTextElement("mText3", QString::number(Period));
        //xmlWriter.writeTextElement("mText4", Clock_text);
    }
    //End Document
    xmlWriter.writeEndDocument();
    //File Close
    file.close();
}

void ScoreboardMain::on_checkBox_clicked(bool checked20)
{
    if(checked20 == true)
    {
        xml();
    }else{
        QFile::remove(".\\Output\\Xml.xml");
        QFile::remove(".\\Update\\out.txt");
    }
}

void ScoreboardMain::on_actionDonate_triggered()
{
    QMessageBox DonateBox;
    DonateBox.setText("<a href=https://goo.gl/fQD3gI>Donate!</a>");
    DonateBox.setInformativeText("Want to support me? I know this is completely optional\nbut it will definitely help me to keep working on this project!\nI don't care how much remember,every little bit counts!");
    DonateBox.exec();
}

void ScoreboardMain::on_HomeName_Input_returnPressed()
{
    if(ui->HomeName_Input->text() == "hello5643")
    {
        SecretIanButton = true;
        ui->Testing->setStyleSheet("QLabel{color: rgb(0, 220, 0);}");
        ui->Testing->setText("Secret Ian Mode Activated!");
        ui->Testing->setStyleSheet("QLabel{color: rgb(0, 0, 0);}");
    }
}

void ScoreboardMain::on_SoundSpeed_Slider_sliderMoved(int slidepos)
{
    qreal test = (qreal)slidepos;
    if(test < 0.0)
    {
        test = -0.9;
    }
    player->setPlaybackRate(test);
}
