#ifndef LUNAR
#define LUNAR

#include <QString>
#include <QDateTime>
#include <QStringList>
// 农历算法来源于网络
// 源:http://www.cnblogs.com/qintangtao/archive/2013/03/01/2938887.html
// 感谢作者：qintangtao
// 修过为Qt hj 2017/04/18
//
// 数据结构如下，共使用17位数据
// 第17位：表示闰月天数，0表示29天   1表示30天
// 第16位-第5位（共12位）表示12个月，其中第16位表示第一月，如果该月为30天则为1，29天为0
// 第4位-第1位（共4位）表示闰月是哪个月，如果当年没有闰月，则置0

#define cn(str) QTextCodec::codecForLocale()->toUnicode(str)

struct SolarHoliday
{
    int month;
    int day;
    int recess;  // 休息长度
    QString holidayName;
    SolarHoliday(int m, int d, int r, QString name)
    {
        month = m;
        day = d;
        recess = r;
        holidayName = name;
    }
};
struct LunarHoliday
{
    int month;
    int day;
    int recess;
    QString holidayName;
    LunarHoliday(int m, int d, int r, QString name)
    {
        month = m;
        day = d;
        recess = r;
        holidayName = name;
    }
};
struct WeekHoliday
{
    int month;
    int weekMonth;
    int weekDay;
    QString holidayName;
    WeekHoliday(int m1, int m2, int d, QString name)
    {
        month = m1;
        weekMonth = m2;
        weekDay = d;
        holidayName = name;
    }
};

static const int LunarDateArray[] =
{
    0x04BD8,0x04AE0,0x0A570,0x054D5,0x0D260,0x0D950,0x16554,0x056A0,0x09AD0,0x055D2,
    0x04AE0,0x0A5B6,0x0A4D0,0x0D250,0x1D255,0x0B540,0x0D6A0,0x0ADA2,0x095B0,0x14977,
    0x04970,0x0A4B0,0x0B4B5,0x06A50,0x06D40,0x1AB54,0x02B60,0x09570,0x052F2,0x04970,
    0x06566,0x0D4A0,0x0EA50,0x06E95,0x05AD0,0x02B60,0x186E3,0x092E0,0x1C8D7,0x0C950,
    0x0D4A0,0x1D8A6,0x0B550,0x056A0,0x1A5B4,0x025D0,0x092D0,0x0D2B2,0x0A950,0x0B557,
    0x06CA0,0x0B550,0x15355,0x04DA0,0x0A5B0,0x14573,0x052B0,0x0A9A8,0x0E950,0x06AA0,
    0x0AEA6,0x0AB50,0x04B60,0x0AAE4,0x0A570,0x05260,0x0F263,0x0D950,0x05B57,0x056A0,
    0x096D0,0x04DD5,0x04AD0,0x0A4D0,0x0D4D4,0x0D250,0x0D558,0x0B540,0x0B6A0,0x195A6,
    0x095B0,0x049B0,0x0A974,0x0A4B0,0x0B27A,0x06A50,0x06D40,0x0AF46,0x0AB60,0x09570,
    0x04AF5,0x04970,0x064B0,0x074A3,0x0EA50,0x06B58,0x055C0,0x0AB60,0x096D5,0x092E0,
    0x0C960,0x0D954,0x0D4A0,0x0DA50,0x07552,0x056A0,0x0ABB7,0x025D0,0x092D0,0x0CAB5,
    0x0A950,0x0B4A0,0x0BAA4,0x0AD50,0x055D9,0x04BA0,0x0A5B0,0x15176,0x052B0,0x0A930,
    0x07954,0x06AA0,0x0AD50,0x05B52,0x04B60,0x0A6E6,0x0A4E0,0x0D260,0x0EA65,0x0D530,
    0x05AA0,0x076A3,0x096D0,0x04BD7,0x04AD0,0x0A4D0,0x1D0B6,0x0D250,0x0D520,0x0DD45,
    0x0B5A0,0x056D0,0x055B2,0x049B0,0x0A577,0x0A4B0,0x0AA50,0x1B255,0x06D20,0x0ADA0,
    0x14B63
};

// 星座名称
static const QString ConstellationName[] =
{
    cn("白羊座"), cn("金牛座"), cn("双子座"),
    cn("巨蟹座"), cn("狮子座"), cn("处女座"),
    cn("天秤座"), cn("天蝎座"), cn("射手座"),
    cn("摩羯座"), cn("水瓶座"), cn("双鱼座")
};

// 二十四节气
static const QString LunarHolidayName[] =
{
    cn("小寒"), cn("大寒"), cn("立春"), cn("雨水"),
    cn("惊蛰"), cn("春分"), cn("清明"), cn("谷雨"),
    cn("立夏"), cn("小满"), cn("芒种"), cn("夏至"),
    cn("小暑"), cn("大暑"), cn("立秋"), cn("处暑"),
    cn("白露"), cn("秋分"), cn("寒露"), cn("霜降"),
    cn("立冬"), cn("小雪"), cn("大雪"), cn("冬至")
};

// 二十八星宿
static const QString ChineseConstellationName[] =
{
      //四        五      六         日        一      二      三
    cn("角木蛟"), cn("亢金龙"), cn("女土蝠"), cn("房日兔"), cn("心月狐"), cn("尾火虎"), cn("箕水豹"),
    cn("斗木獬"), cn("牛金牛"), cn("氐土貉"), cn("虚日鼠"), cn("危月燕"), cn("室火猪"), cn("壁水獝"),
    cn("奎木狼"), cn("娄金狗"), cn("胃土彘"), cn("昴日鸡"), cn("毕月乌"), cn("觜火猴"), cn("参水猿"),
    cn("井木犴"), cn("鬼金羊"), cn("柳土獐"), cn("星日马"), cn("张月鹿"), cn("翼火蛇"), cn("轸水蚓")
};

static const QString SolarTerm[] =
{
    cn("小寒"), cn("大寒"), cn("立春"), cn("雨水"),
    cn("惊蛰"), cn("春分"), cn("清明"), cn("谷雨"),
    cn("立夏"), cn("小满"), cn("芒种"), cn("夏至"),
    cn("小暑"), cn("大暑"), cn("立秋"), cn("处暑"),
    cn("白露"), cn("秋分"), cn("寒露"), cn("霜降"),
    cn("立冬"), cn("小雪"), cn("大雪"), cn("冬至")
};

static const int sTermInfo[] =
{
    0, 21208, 42467, 63836, 85337, 107014, 128867, 150921, 173149,
    195551, 218072, 240693, 263343, 285989, 308563, 331033, 353350,
    375494, 397447, 419210, 440795, 462224, 483532, 504758
};

static const QString ganStr = cn("甲乙丙丁戊己庚辛壬癸");
static const QString zhiStr = cn("子丑寅卯辰巳午未申酉戌亥");
static const QString animalStr = cn("鼠牛虎兔龙蛇马羊猴鸡狗猪");
static const QString nStr1 = cn("日一二三四五六七八九");
static const QString nStr2 = cn("初十廿卅");
static const QString weekStr = cn("一二三四五六日");
static const QString MonthString[] =
{
    cn("出错"), cn("正月"), cn("二月"), cn("三月"), cn("四月"), cn("五月"), cn("六月"),
    cn("七月"), cn("八月"), cn("九月"), cn("十月"), cn("十一月"), cn("腊月")
};

//  按公历计算的节日
static const SolarHoliday sHolidayInfo[] =
{
    SolarHoliday(1, 1, 1, cn("元旦")),
    SolarHoliday(2, 2, 0, cn("世界湿地日")),
    SolarHoliday(2, 10, 0, cn("国际气象节")),
    SolarHoliday(2, 14, 0, cn("情人节")),
    SolarHoliday(3, 1, 0, cn("国际海豹日")),
    SolarHoliday(3, 5, 0, cn("学雷锋纪念日")),
    SolarHoliday(3, 8, 0, cn("妇女节")),
    SolarHoliday(3, 12, 0, cn("植树节 孙中山逝世纪念日")),
    SolarHoliday(3, 14, 0, cn("国际警察日")),
    SolarHoliday(3, 15, 0, cn("消费者权益日")),
    SolarHoliday(3, 17, 0, cn("中国国医节 国际航海日")),
    SolarHoliday(3, 21, 0, cn("世界森林日 消除种族歧视国际日 世界儿歌日")),
    SolarHoliday(3, 22, 0, cn("世界水日")),
    SolarHoliday(3, 24, 0, cn("世界防治结核病日")),
    SolarHoliday(4, 1, 0, cn("愚人节")),
    SolarHoliday(4, 7, 0, cn("世界卫生日")),
    SolarHoliday(4, 22, 0, cn("世界地球日")),
    SolarHoliday(5, 1, 1, cn("劳动节")),
    SolarHoliday(5, 2, 1, cn("劳动节假日")),
    SolarHoliday(5, 3, 1, cn("劳动节假日")),
    SolarHoliday(5, 4, 0, cn("青年节")),
    SolarHoliday(5, 8, 0, cn("世界红十字日")),
    SolarHoliday(5, 12, 0, cn("国际护士节")),
    SolarHoliday(5, 31, 0, cn("世界无烟日")),
    SolarHoliday(6, 1, 0, cn("国际儿童节")),
    SolarHoliday(6, 5, 0, cn("世界环境保护日")),
    SolarHoliday(6, 26, 0, cn("国际禁毒日")),
    SolarHoliday(7, 1, 0, cn("建党节 香港回归纪念 世界建筑日")),
    SolarHoliday(7, 11, 0, cn("世界人口日")),
    SolarHoliday(8, 1, 0, cn("建军节")),
    SolarHoliday(8, 8, 0, cn("中国男子节 父亲节")),
    SolarHoliday(8, 15, 0, cn("抗日战争胜利纪念")),
    SolarHoliday(9, 9, 0, cn("逝世纪念")),
    SolarHoliday(9, 10, 0, cn("教师节")),
    SolarHoliday(9, 18, 0, cn("九·一八事变纪念日")),
    SolarHoliday(9, 20, 0, cn("国际爱牙日")),
    SolarHoliday(9, 27, 0, cn("世界旅游日")),
    SolarHoliday(9, 28, 0, cn("孔子诞辰")),
    SolarHoliday(10, 1, 1, cn("国庆节 国际音乐日")),
    SolarHoliday(10, 2, 1, cn("国庆节假日")),
    SolarHoliday(10, 3, 1, cn("国庆节假日")),
    SolarHoliday(10, 6, 0, cn("老人节")),
    SolarHoliday(10, 24, 0, cn("联合国日")),
    SolarHoliday(11, 10, 0, cn("世界青年节")),
    SolarHoliday(11, 12, 0, cn("孙中山诞辰纪念")),
    SolarHoliday(12, 1, 0, cn("世界艾滋病日")),
    SolarHoliday(12, 3, 0, cn("世界残疾人日")),
    SolarHoliday(12, 20, 0, cn("澳门回归纪念")),
    SolarHoliday(12, 24, 0, cn("平安夜")),
    SolarHoliday(12, 25, 0, cn("圣诞节")),
    SolarHoliday(12, 26, 0, cn(" 诞辰纪念"))
};

// 按农历计算的节日
static LunarHoliday lHolidayInfo[] =
{
    LunarHoliday(1, 1, 1, cn("春节")),
    LunarHoliday(1, 15, 0, cn("元宵节")),
    LunarHoliday(5, 5, 0, cn("端午节")),
    LunarHoliday(7, 7, 0, cn("七夕情人节")),
    LunarHoliday(7, 15, 0, cn("中元节 盂兰盆节")),
    LunarHoliday(8, 15, 0, cn("中秋节")),
    LunarHoliday(9, 9, 0, cn("重阳节")),
    LunarHoliday(12, 8, 0, cn("腊八节")),
    LunarHoliday(12, 23, 0, cn("北方小年(扫房)")),
    LunarHoliday(12, 24, 0, cn("南方小年(掸尘)")),
    //LunarHolidayStruct(12, 30, 0, "除夕")  //注意除夕需要其它方法进行计算
};

// 按某月第几个星期几
static const WeekHoliday wHolidayInfo[] =
{
    WeekHoliday(5, 2, 1, cn("母亲节")),
    WeekHoliday(5, 3, 1, cn("全国助残日")),
    WeekHoliday(6, 3, 1, cn("父亲节")),
    WeekHoliday(9, 3, 3, cn("国际和平日")),
    WeekHoliday(9, 4, 1, cn("国际聋人节")),
    WeekHoliday(10, 1, 2, cn("国际住房日")),
    WeekHoliday(10, 1, 4, cn("国际减轻自然灾害日")),
    WeekHoliday(11, 4, 5, cn("感恩节"))
};

static const QString HZNum = cn("零一二三四五六七八九");
static const int AnimalStartYear = 1900; //1900年为鼠年

// 公历转农历
class ChineseCalendar
{
public:
    ChineseCalendar(QDateTime dt)
    {
        minDate.setDate(1900, 1, 30);
        maxDate.setDate(2049, 12, 31);
        minYear = 1900;
        maxYear = 2050;
        ganZhiStartYear = 1864; //干支计算起始年
        ganZhiStartDt.setDate(1899, 12, 22);
        ChineseConstellationReferDay.setDate(2007, 9, 13); //28星宿参考值,本日为角
        curDate = dt.date();
        curDatetime = dt;
    }

    QString getChineseDateString()
    {
        QString str = cn("农历");
        if (isLeapMonth)
        {
            str = str + getChineseYearString() + cn("闰") +
                  getChineseMonthString() +
                  getChineseDayString();
        }
        else
        {
            str = str + getChineseYearString() +
                  getChineseMonthString() +
                  getChineseDayString();
        }
        return str;
    }

    // 农历年份
    int getChineseYear()
    {
        return year;
    }

    // 农历年字符串如，一九九七年
    QString getChineseYearString()
    {
        QString tempStr = "";
        for (int i = 0; i < 4; i++)
        {
            tempStr.append(convertNumToChineseNum(QString(QString::number(year).at(i)).toInt()));
        }
        return tempStr.append(cn("年"));
    }

    // 农历的月份
    int getChineseMonth()
    {
        return month;
    }

    // 农历月份字符串
    QString getChineseMonthString()
    {
        return MonthString[month];
    }

    // 农历日
    int getChineseDay()
    {
        return day;
    }
    // 农历日中文表示
    QString getChineseDayString()
    {
        switch (day)
        {
            case 0:
                return "";
            case 10:
                return cn("初十");
            case 20:
                return cn("二十");
            case 30:
                return cn("三十");
            default:
                return QString(nStr2[day / 10]) + QString(nStr1[day % 10]);

        }
    }

    // 定气法计算二十四节气,二十四节气是按地球公转来计算的，并非是阴历计算的
    // 节气的定法有两种。古代历法采用的称为"恒气"，即按时间把一年等分为24份，
    // 每一节气平均得15天有余，所以又称"平气"。现代农历采用的称为"定气"，即
    // 按地球在轨道上的位置为标准，一周360°，两节气之间相隔15°。由于冬至时地
    // 球位于近日点附近，运动速度较快，因而太阳在黄道上移动15°的时间不到15天。
    // 夏至前后的情况正好相反，太阳在黄道上移动较慢，一个节气达16天之多。采用
    // 定气时可以保证春、秋两分必然在昼夜平分的那两天
    QString getChineseTwentyFourDay()
    {
        QDateTime dt1(QDate(1900, 1, 6), QTime(2, 5));
        QDateTime dt2;
        double num;
        int y;
        QString tempStr = "";
        y = curDate.year();
        for (int i = 1; i <= 24; i++)
        {
            num = 525948.76 * (y - 1900) + sTermInfo[i - 1];
            dt2 = dt1.addSecs(num * 60);
            if (dt2.date().dayOfYear() == curDate.dayOfYear())
            {
                tempStr = SolarTerm[i - 1];
                break;
            }
        }
        return tempStr;
    }

    // 当前日期前一个最近节气
    QString getChineseTwentyFourPrevDay()
    {
        QString str = getChineseTwentyFourDay();
        if (str.isEmpty())
        {
            QDateTime dt1(QDate(1900, 1, 6), QTime(2, 5));
            QDateTime dt2;
            double num;
            int y;
            QString tempStr = "";
            y = curDate.year();
            for (int i = 1; i <= 24; i++)
            {
                num = 525948.76 * (y - 1900) + sTermInfo[i - 1];
                dt2 = dt1.addSecs(num * 60);
                if (dt2.date().dayOfYear() <  curDate.dayOfYear())
                {
                    tempStr = SolarTerm[i - 1] + dt2.toString(" yyyy-MM-dd");;
//                    break;
                }
            }
            if (tempStr.isEmpty())
                tempStr = cn("冬至");
            return tempStr;
        }
        else
        {
            QStringList list;
            for (QString s : SolarTerm)
            {
                list << s;
            }
            for (int i = 0; i < list.size(); i++)
            {
                if (str == list.at(i))
                {
                    if (i == 0)
                        str = list.first();
                    else if (i == list.size() - 1)
                        str = list.last();
                    else
                        str = list.at(i - 1);
                    break;
                }
            }
            return str;
        }
    }

    // 当前日期后一个最近节气
    QString getChineseTwentyFourNextDay()
    {
        QString str = getChineseTwentyFourDay();
        if (str.isEmpty())
        {
            QDateTime dt1(QDate(1900, 1, 6), QTime(2, 5));
            QDateTime dt2;
            double num;
            int y;
            QString tempStr = "";
            y = curDate.year();
            for (int i = 1; i <= 24; i++)
            {
                num = 525948.76 * (y - 1900) + sTermInfo[i - 1];
                dt2 = dt1.addSecs(num * 60);
                if (dt2.date().dayOfYear() > curDate.dayOfYear())
                {
                    tempStr = dt2.toString("yyyy-MM-dd ") + SolarTerm[i - 1];
                    break;
                }
            }
            if (tempStr.isEmpty())
                tempStr = cn("小寒");
            return tempStr;
        }
        else
        {
            QStringList list;
            for (QString s : SolarTerm)
            {
                list << s;
            }
            for (int i = 0; i < list.size(); i++)
            {
                if (str == list.at(i))
                {
                    if (i == 0)
                        str = list.last();
                    else if (i == list.size() - 1)
                        str = list.first();
                    else
                        str = list.at(i + 1);
                    break;
                }
            }
            return str;
        }
    }

    // 计算指定日期的星座序号
    QString getConstellation()
    {
        int index = 0;
        int y, m, d;
        y = curDate.year();
        m = curDate.month();
        d = curDate.day();
        y = m * 100 + d;

        if (((y >= 321) && (y <= 419))) { index = 0; }
        else if ((y >= 420) && (y <= 520)) { index = 1; }
        else if ((y >= 521) && (y <= 620)) { index = 2; }
        else if ((y >= 621) && (y <= 722)) { index = 3; }
        else if ((y >= 723) && (y <= 822)) { index = 4; }
        else if ((y >= 823) && (y <= 922)) { index = 5; }
        else if ((y >= 923) && (y <= 1022)) { index = 6; }
        else if ((y >= 1023) && (y <= 1121)) { index = 7; }
        else if ((y >= 1122) && (y <= 1221)) { index = 8; }
        else if ((y >= 1222) || (y <= 119)) { index = 9; }
        else if ((y >= 120) && (y <= 218)) { index = 10; }
        else if ((y >= 219) && (y <= 320)) { index = 11; }
        else { index = 0; }

        return ConstellationName[index];
    }

    // 属相字符串
    QString getAnimalString()
    {
        if (year - AnimalStartYear < 0)
            return "";
        return animalStr[(year - AnimalStartYear) % 12];
    }

    // 农历年的干支表示法如 乙丑年
    QString getGanZhiYearString()
    {
        return QString(ganStr[(year - ganZhiStartYear) % 10]) +
               QString(zhiStr[(year - ganZhiStartYear) % 12]) + cn("年");
    }

    // 干支的月表示字符串，注意农历的闰月不记干支
    QString getGanZhiMonthString()
    {
        //每个月的地支总是固定的,而且总是从寅月开始
        int zhiIndex;
        QString zhi;

        if (month > 10)
        {
            zhiIndex = month - 10;
        }
        else
        {
            zhiIndex = month + 2;
        }
        zhi = zhiStr[zhiIndex - 1];

        //根据当年的干支年的干来计算月干的第一个
        int ganIndex = 1;
        QString gan;
        int i = (year - ganZhiStartYear) % 60; //计算干支
        switch (i % 10)
        {
            case 0: //甲
                ganIndex = 3;
                break;
            case 1: //乙
                ganIndex = 5;
                break;
            case 2: //丙
                ganIndex = 7;
                break;
            case 3: //丁
                ganIndex = 9;
                break;
            case 4: //戊
                ganIndex = 1;
                break;
            case 5: //己
                ganIndex = 3;
                break;
            case 6: //庚
                ganIndex = 5;
                break;
            case 7: //辛
                ganIndex = 7;
                break;
            case 8: //壬
                ganIndex = 9;
                break;
            case 9: //癸
                ganIndex = 1;
                break;
        }
        gan = ganStr[(ganIndex + month - 2) % 10];

        return gan + zhi + cn("月");
    }

    // 干支日表示法
    QString getGanZhiDayString()
    {
        int i = ganZhiStartDt.daysTo(curDate) % 60;
        return QString(ganStr[i % 10]) + QString(zhiStr[i % 12]) + cn("日");
    }

    // 当前日期的干支表示法如 甲子年乙丑月丙庚日
    QString getGanZhiDateString()
    {
        return getGanZhiYearString() + getGanZhiMonthString() + getGanZhiDayString();
    }

    // 时辰
    QString getChineseHour()
    {

        int _hour, _minute, offset, i;
        int indexGan;
        //string ganHour, zhiHour;
        QString tmpGan;

        //计算时辰的地支
        _hour = curDatetime.time().hour();    //获得当前时间小时
        _minute = curDatetime.time().minute();  //获得当前时间分钟

        if (_minute != 0)
            _hour += 1;
        offset = _hour / 2;
        if (offset >= 12)
            offset = 0;

        //计算天干
        i = ganZhiStartDt.daysTo(curDate) % 60;

        indexGan = ((i % 10 + 1) * 2 - 1) % 10 - 1; //ganStr[i % 10] 为日的天干,(n*2-1) %10得出地支对应,n从1开始
        tmpGan = ganStr.mid(indexGan) + ganStr.mid(0, indexGan + 2);//凑齐12位

        return QString(tmpGan[offset]) + QString(zhiStr[offset]);
    }
    // 计算中国农历节日
    QString getChineseCalendarHoliday()
    {
        QString tempStr = "";
        if (!isLeapMonth)
        {
            for(LunarHoliday lh : lHolidayInfo)
            {
                if (lh.month == month && lh.day == day)
                {
                    tempStr = lh.holidayName;
                    break;
                }
            }
            //对除夕进行特别处理
            if (month == 12)
            {
                int i = getChineseMonthDays(year, 12);
                if (day == i)  // 最后一天
                {
                    tempStr = cn("除夕");
                }
            }
        }
        return tempStr;
    }

    // 按某月第几周第几日计算的节日
    QString getWeekDayHoliday()
    {
        QString tempStr = "";
        for (WeekHoliday wh : wHolidayInfo)
        {
            if (compareWeekDayHoliday(curDate, wh.month, wh.weekMonth, wh.weekDay))
            {
                tempStr = wh.holidayName;
                break;
            }
        }
        return tempStr;
    }

    // 按公历日计算的节日
    QString getDateHoliday()
    {
        QString tempStr = "";
        for (SolarHoliday sh : sHolidayInfo)
        {
            if (sh.month == curDate.month() && sh.day == curDate.day())
            {
                tempStr = sh.holidayName;
                break;
            }
        }
        return tempStr;
    }

    // 星期几的字符
    QString getWeekDayStr()
    {
        QString str = cn("星期");
        return str.append(weekStr[curDate.dayOfWeek() - 1]);
    }

    // 28星宿计算
    QString getChineseConstellation()
    {
        int offset = 0;
        int modStarDay = 0;
        offset = ChineseConstellationReferDay.daysTo(curDate);
        modStarDay = offset % 28;
        return modStarDay >= 0 ? ChineseConstellationName[modStarDay] :
            ChineseConstellationName[27 + modStarDay];
    }

    // 是否闰月
    bool isChineseLeapMonth()
    {
        return isLeapMonth;
    }

    // 当年是否有闰月
    bool isChineseLeapYear()
    {
        return isLeapYear;
    }

    bool calc()
    {
        if (curDate < minDate || curDate > maxDate)
        {
            return false;
        }
        int i = 0;
        int leap = 0;
        int temp = 0;
        int offset = 0;
        offset = minDate.daysTo(curDate);
        for (i = minYear; i <= maxYear; i++)
        {
            temp = getChineseYearDays(i);
            if (offset - temp < 1)
                break;
            else
                offset = offset - temp;
        }
        year = i;
        leap = getChineseLeapMonth(year);
        if (leap > 0)
            isLeapYear = true;
        else
            isLeapYear = false;
        isLeapMonth = false;
        for (i = 1; i <= 12; i++)
        {
            // 闰月
            if ((leap > 0) && (i == leap + 1) && (isLeapMonth == false))
            {
                isLeapMonth = true;
                i = i -1;
                temp = getChineseLeapMonthDays(year); //计算闰月天数
            }
            else
            {
                isLeapMonth = false;
                temp = getChineseMonthDays(year, i); //计算非闰月天数
            }
            offset = offset - temp;
            if (offset <= 0)
                break;
        }
        offset = offset + temp;
        month = i;
        day = offset;
        return true;
    }

private:
    int getChineseYearDays(int year)
    {
        int i, f, sumDay, info;
        sumDay = 348; //29天 X 12个月
        i = 0x8000;
        info = LunarDateArray[year - minYear] & 0x0FFFF;
        //0x04BD8  & 0x0FFFF 中间12位，即4BD，每位代表一个月，为1则为大月，为0则为小月
        //计算12个月中有多少天为30天
        for (int m = 0; m < 12; m++)
        {
            f = info & i; // 0x04BD8  & 0x0FFFF  & 0x8000[1000 0000 0000 0000]
            if (f != 0)
            {
                sumDay++;
            }
            i = i >> 1;
        }
        return sumDay + getChineseLeapMonthDays(year);
    }
    int getChineseLeapMonth(int year)
    {
        return LunarDateArray[year - minYear] & 0xF;
    }
    int getChineseLeapMonthDays(int year)
    {
        if (getChineseLeapMonth(year) != 0)
        {
            if ((LunarDateArray[year - minYear] & 0x10000) != 0)
                return 30;
            else
                return 29;
        }
        else
            return 0;
    }
    int getChineseMonthDays(int year, int month)
    {
        if (bitTest32((LunarDateArray[year - minYear] & 0x0000FFFF), (16 - month)))
            return 30;
        else
            return 29;
    }

    bool bitTest32(int num, int pos)
    {
        if (pos > 31 || pos < 0)
            return false;
        int bit = 1 << pos;
        if ((num & bit) == 0)
            return false;
        else
            return true;
    }

    QString convertNumToChineseNum(int n)
    {
        if ((n < 0) || (n > 9))
            return "";
        else
        {
            return HZNum[n];
        }
    }
    bool compareWeekDayHoliday(QDate date, int month, int week, int day)
    {
        bool ret = false;
        if (date.month() == month) //月份相同
        {
            if (date.dayOfWeek() == day) //星期几相同
            {
                QDate firstDay(date.year(), date.month(), 1);//生成当月第一天
                int i = firstDay.dayOfWeek();
                int firWeekDays = 7 - firstDay.dayOfWeek() + 1; //计算第一周剩余天数
                if (i > day)
                {
                    if ((week - 1) * 7 + day + firWeekDays == date.day())
                    {
                        ret = true;
                    }
                }
                else
                {
                    if (day + firWeekDays + (week - 2) * 7 == date.day())
                    {
                        ret = true;
                    }
                }
            }
        }
        return ret;
    }

private:
    QDate curDate;
    QDateTime curDatetime;
    QDate minDate;
    QDate maxDate;
    int minYear;
    int maxYear;
    int ganZhiStartYear;
    QDate ganZhiStartDt;
    QDate ChineseConstellationReferDay;


    int year;
    int month; // 农历月份
    int day;    // 农历月中天
    bool isLeapMonth; // 当月是否闰月
    bool isLeapYear;    // 当年是否闰年
};

#endif // LUNAR
