#ifndef LUNAR
#define LUNAR

#include <QString>
#include <QDateTime>
#include <QStringList>
// ũ���㷨��Դ������
// Դ:http://www.cnblogs.com/qintangtao/archive/2013/03/01/2938887.html
// ��л���ߣ�qintangtao
// �޹�ΪQt hj 2017/04/18
//
// ���ݽṹ���£���ʹ��17λ����
// ��17λ����ʾ����������0��ʾ29��   1��ʾ30��
// ��16λ-��5λ����12λ����ʾ12���£����е�16λ��ʾ��һ�£��������Ϊ30����Ϊ1��29��Ϊ0
// ��4λ-��1λ����4λ����ʾ�������ĸ��£��������û�����£�����0

#define cn(str) QTextCodec::codecForLocale()->toUnicode(str)

struct SolarHoliday
{
    int month;
    int day;
    int recess;  // ��Ϣ����
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

// ��������
static const QString ConstellationName[] =
{
    cn("������"), cn("��ţ��"), cn("˫����"),
    cn("��з��"), cn("ʨ����"), cn("��Ů��"),
    cn("�����"), cn("��Ы��"), cn("������"),
    cn("Ħ����"), cn("ˮƿ��"), cn("˫����")
};

// ��ʮ�Ľ���
static const QString LunarHolidayName[] =
{
    cn("С��"), cn("��"), cn("����"), cn("��ˮ"),
    cn("����"), cn("����"), cn("����"), cn("����"),
    cn("����"), cn("С��"), cn("â��"), cn("����"),
    cn("С��"), cn("����"), cn("����"), cn("����"),
    cn("��¶"), cn("���"), cn("��¶"), cn("˪��"),
    cn("����"), cn("Сѩ"), cn("��ѩ"), cn("����")
};

// ��ʮ������
static const QString ChineseConstellationName[] =
{
      //��        ��      ��         ��        һ      ��      ��
    cn("��ľ��"), cn("������"), cn("Ů����"), cn("������"), cn("���º�"), cn("β��"), cn("��ˮ��"),
    cn("��ľ�"), cn("ţ��ţ"), cn("ص����"), cn("������"), cn("Σ����"), cn("�һ���"), cn("��ˮ��"),
    cn("��ľ��"), cn("¦��"), cn("θ����"), cn("���ռ�"), cn("������"), cn("�����"), cn("��ˮԳ"),
    cn("��ľ��"), cn("�����"), cn("�����"), cn("������"), cn("����¹"), cn("�����"), cn("��ˮ�")
};

static const QString SolarTerm[] =
{
    cn("С��"), cn("��"), cn("����"), cn("��ˮ"),
    cn("����"), cn("����"), cn("����"), cn("����"),
    cn("����"), cn("С��"), cn("â��"), cn("����"),
    cn("С��"), cn("����"), cn("����"), cn("����"),
    cn("��¶"), cn("���"), cn("��¶"), cn("˪��"),
    cn("����"), cn("Сѩ"), cn("��ѩ"), cn("����")
};

static const int sTermInfo[] =
{
    0, 21208, 42467, 63836, 85337, 107014, 128867, 150921, 173149,
    195551, 218072, 240693, 263343, 285989, 308563, 331033, 353350,
    375494, 397447, 419210, 440795, 462224, 483532, 504758
};

static const QString ganStr = cn("���ұ����켺�����ɹ�");
static const QString zhiStr = cn("�ӳ���î������δ�����纥");
static const QString animalStr = cn("��ţ������������Ｆ����");
static const QString nStr1 = cn("��һ�����������߰˾�");
static const QString nStr2 = cn("��ʮإئ");
static const QString weekStr = cn("һ������������");
static const QString MonthString[] =
{
    cn("����"), cn("����"), cn("����"), cn("����"), cn("����"), cn("����"), cn("����"),
    cn("����"), cn("����"), cn("����"), cn("ʮ��"), cn("ʮһ��"), cn("����")
};

//  ����������Ľ���
static const SolarHoliday sHolidayInfo[] =
{
    SolarHoliday(1, 1, 1, cn("Ԫ��")),
    SolarHoliday(2, 2, 0, cn("����ʪ����")),
    SolarHoliday(2, 10, 0, cn("���������")),
    SolarHoliday(2, 14, 0, cn("���˽�")),
    SolarHoliday(3, 1, 0, cn("���ʺ�����")),
    SolarHoliday(3, 5, 0, cn("ѧ�׷������")),
    SolarHoliday(3, 8, 0, cn("��Ů��")),
    SolarHoliday(3, 12, 0, cn("ֲ���� ����ɽ����������")),
    SolarHoliday(3, 14, 0, cn("���ʾ�����")),
    SolarHoliday(3, 15, 0, cn("������Ȩ����")),
    SolarHoliday(3, 17, 0, cn("�й���ҽ�� ���ʺ�����")),
    SolarHoliday(3, 21, 0, cn("����ɭ���� �����������ӹ����� ���������")),
    SolarHoliday(3, 22, 0, cn("����ˮ��")),
    SolarHoliday(3, 24, 0, cn("������ν�˲���")),
    SolarHoliday(4, 1, 0, cn("���˽�")),
    SolarHoliday(4, 7, 0, cn("����������")),
    SolarHoliday(4, 22, 0, cn("���������")),
    SolarHoliday(5, 1, 1, cn("�Ͷ���")),
    SolarHoliday(5, 2, 1, cn("�Ͷ��ڼ���")),
    SolarHoliday(5, 3, 1, cn("�Ͷ��ڼ���")),
    SolarHoliday(5, 4, 0, cn("�����")),
    SolarHoliday(5, 8, 0, cn("�����ʮ����")),
    SolarHoliday(5, 12, 0, cn("���ʻ�ʿ��")),
    SolarHoliday(5, 31, 0, cn("����������")),
    SolarHoliday(6, 1, 0, cn("���ʶ�ͯ��")),
    SolarHoliday(6, 5, 0, cn("���绷��������")),
    SolarHoliday(6, 26, 0, cn("���ʽ�����")),
    SolarHoliday(7, 1, 0, cn("������ ��ۻع���� ���罨����")),
    SolarHoliday(7, 11, 0, cn("�����˿���")),
    SolarHoliday(8, 1, 0, cn("������")),
    SolarHoliday(8, 8, 0, cn("�й����ӽ� ���׽�")),
    SolarHoliday(8, 15, 0, cn("����ս��ʤ������")),
    SolarHoliday(9, 9, 0, cn("��������")),
    SolarHoliday(9, 10, 0, cn("��ʦ��")),
    SolarHoliday(9, 18, 0, cn("�š�һ���±������")),
    SolarHoliday(9, 20, 0, cn("���ʰ�����")),
    SolarHoliday(9, 27, 0, cn("����������")),
    SolarHoliday(9, 28, 0, cn("���ӵ���")),
    SolarHoliday(10, 1, 1, cn("����� ����������")),
    SolarHoliday(10, 2, 1, cn("����ڼ���")),
    SolarHoliday(10, 3, 1, cn("����ڼ���")),
    SolarHoliday(10, 6, 0, cn("���˽�")),
    SolarHoliday(10, 24, 0, cn("���Ϲ���")),
    SolarHoliday(11, 10, 0, cn("���������")),
    SolarHoliday(11, 12, 0, cn("����ɽ��������")),
    SolarHoliday(12, 1, 0, cn("���簬�̲���")),
    SolarHoliday(12, 3, 0, cn("����м�����")),
    SolarHoliday(12, 20, 0, cn("���Żع����")),
    SolarHoliday(12, 24, 0, cn("ƽ��ҹ")),
    SolarHoliday(12, 25, 0, cn("ʥ����")),
    SolarHoliday(12, 26, 0, cn(" ��������"))
};

// ��ũ������Ľ���
static LunarHoliday lHolidayInfo[] =
{
    LunarHoliday(1, 1, 1, cn("����")),
    LunarHoliday(1, 15, 0, cn("Ԫ����")),
    LunarHoliday(5, 5, 0, cn("�����")),
    LunarHoliday(7, 7, 0, cn("��Ϧ���˽�")),
    LunarHoliday(7, 15, 0, cn("��Ԫ�� �������")),
    LunarHoliday(8, 15, 0, cn("�����")),
    LunarHoliday(9, 9, 0, cn("������")),
    LunarHoliday(12, 8, 0, cn("���˽�")),
    LunarHoliday(12, 23, 0, cn("����С��(ɨ��)")),
    LunarHoliday(12, 24, 0, cn("�Ϸ�С��(����)")),
    //LunarHolidayStruct(12, 30, 0, "��Ϧ")  //ע���Ϧ��Ҫ�����������м���
};

// ��ĳ�µڼ������ڼ�
static const WeekHoliday wHolidayInfo[] =
{
    WeekHoliday(5, 2, 1, cn("ĸ�׽�")),
    WeekHoliday(5, 3, 1, cn("ȫ��������")),
    WeekHoliday(6, 3, 1, cn("���׽�")),
    WeekHoliday(9, 3, 3, cn("���ʺ�ƽ��")),
    WeekHoliday(9, 4, 1, cn("�������˽�")),
    WeekHoliday(10, 1, 2, cn("����ס����")),
    WeekHoliday(10, 1, 4, cn("���ʼ�����Ȼ�ֺ���")),
    WeekHoliday(11, 4, 5, cn("�ж���"))
};

static const QString HZNum = cn("��һ�����������߰˾�");
static const int AnimalStartYear = 1900; //1900��Ϊ����

// ����תũ��
class ChineseCalendar
{
public:
    ChineseCalendar(QDateTime dt)
    {
        minDate.setDate(1900, 1, 30);
        maxDate.setDate(2049, 12, 31);
        minYear = 1900;
        maxYear = 2050;
        ganZhiStartYear = 1864; //��֧������ʼ��
        ganZhiStartDt.setDate(1899, 12, 22);
        ChineseConstellationReferDay.setDate(2007, 9, 13); //28���޲ο�ֵ,����Ϊ��
        curDate = dt.date();
        curDatetime = dt;
    }

    QString getChineseDateString()
    {
        QString str = cn("ũ��");
        if (isLeapMonth)
        {
            str = str + getChineseYearString() + cn("��") +
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

    // ũ�����
    int getChineseYear()
    {
        return year;
    }

    // ũ�����ַ����磬һ�ž�����
    QString getChineseYearString()
    {
        QString tempStr = "";
        for (int i = 0; i < 4; i++)
        {
            tempStr.append(convertNumToChineseNum(QString(QString::number(year).at(i)).toInt()));
        }
        return tempStr.append(cn("��"));
    }

    // ũ�����·�
    int getChineseMonth()
    {
        return month;
    }

    // ũ���·��ַ���
    QString getChineseMonthString()
    {
        return MonthString[month];
    }

    // ũ����
    int getChineseDay()
    {
        return day;
    }
    // ũ�������ı�ʾ
    QString getChineseDayString()
    {
        switch (day)
        {
            case 0:
                return "";
            case 10:
                return cn("��ʮ");
            case 20:
                return cn("��ʮ");
            case 30:
                return cn("��ʮ");
            default:
                return QString(nStr2[day / 10]) + QString(nStr1[day % 10]);

        }
    }

    // �����������ʮ�Ľ���,��ʮ�Ľ����ǰ�����ת������ģ����������������
    // �����Ķ��������֡��Ŵ��������õĳ�Ϊ"����"������ʱ���һ��ȷ�Ϊ24�ݣ�
    // ÿһ����ƽ����15�����࣬�����ֳ�"ƽ��"���ִ�ũ�����õĳ�Ϊ"����"����
    // �������ڹ���ϵ�λ��Ϊ��׼��һ��360�㣬������֮�����15�㡣���ڶ���ʱ��
    // ��λ�ڽ��յ㸽�����˶��ٶȽϿ죬���̫���ڻƵ����ƶ�15���ʱ�䲻��15�졣
    // ����ǰ�����������෴��̫���ڻƵ����ƶ�������һ��������16��֮�ࡣ����
    // ����ʱ���Ա�֤���������ֱ�Ȼ����ҹƽ�ֵ�������
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

    // ��ǰ����ǰһ���������
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
                tempStr = cn("����");
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

    // ��ǰ���ں�һ���������
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
                tempStr = cn("С��");
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

    // ����ָ�����ڵ��������
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

    // �����ַ���
    QString getAnimalString()
    {
        if (year - AnimalStartYear < 0)
            return "";
        return animalStr[(year - AnimalStartYear) % 12];
    }

    // ũ����ĸ�֧��ʾ���� �ҳ���
    QString getGanZhiYearString()
    {
        return QString(ganStr[(year - ganZhiStartYear) % 10]) +
               QString(zhiStr[(year - ganZhiStartYear) % 12]) + cn("��");
    }

    // ��֧���±�ʾ�ַ�����ע��ũ�������²��Ǹ�֧
    QString getGanZhiMonthString()
    {
        //ÿ���µĵ�֧���ǹ̶���,�������Ǵ����¿�ʼ
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

        //���ݵ���ĸ�֧��ĸ��������¸ɵĵ�һ��
        int ganIndex = 1;
        QString gan;
        int i = (year - ganZhiStartYear) % 60; //�����֧
        switch (i % 10)
        {
            case 0: //��
                ganIndex = 3;
                break;
            case 1: //��
                ganIndex = 5;
                break;
            case 2: //��
                ganIndex = 7;
                break;
            case 3: //��
                ganIndex = 9;
                break;
            case 4: //��
                ganIndex = 1;
                break;
            case 5: //��
                ganIndex = 3;
                break;
            case 6: //��
                ganIndex = 5;
                break;
            case 7: //��
                ganIndex = 7;
                break;
            case 8: //��
                ganIndex = 9;
                break;
            case 9: //��
                ganIndex = 1;
                break;
        }
        gan = ganStr[(ganIndex + month - 2) % 10];

        return gan + zhi + cn("��");
    }

    // ��֧�ձ�ʾ��
    QString getGanZhiDayString()
    {
        int i = ganZhiStartDt.daysTo(curDate) % 60;
        return QString(ganStr[i % 10]) + QString(zhiStr[i % 12]) + cn("��");
    }

    // ��ǰ���ڵĸ�֧��ʾ���� �������ҳ��±�����
    QString getGanZhiDateString()
    {
        return getGanZhiYearString() + getGanZhiMonthString() + getGanZhiDayString();
    }

    // ʱ��
    QString getChineseHour()
    {

        int _hour, _minute, offset, i;
        int indexGan;
        //string ganHour, zhiHour;
        QString tmpGan;

        //����ʱ���ĵ�֧
        _hour = curDatetime.time().hour();    //��õ�ǰʱ��Сʱ
        _minute = curDatetime.time().minute();  //��õ�ǰʱ�����

        if (_minute != 0)
            _hour += 1;
        offset = _hour / 2;
        if (offset >= 12)
            offset = 0;

        //�������
        i = ganZhiStartDt.daysTo(curDate) % 60;

        indexGan = ((i % 10 + 1) * 2 - 1) % 10 - 1; //ganStr[i % 10] Ϊ�յ����,(n*2-1) %10�ó���֧��Ӧ,n��1��ʼ
        tmpGan = ganStr.mid(indexGan) + ganStr.mid(0, indexGan + 2);//����12λ

        return QString(tmpGan[offset]) + QString(zhiStr[offset]);
    }
    // �����й�ũ������
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
            //�Գ�Ϧ�����ر���
            if (month == 12)
            {
                int i = getChineseMonthDays(year, 12);
                if (day == i)  // ���һ��
                {
                    tempStr = cn("��Ϧ");
                }
            }
        }
        return tempStr;
    }

    // ��ĳ�µڼ��ܵڼ��ռ���Ľ���
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

    // �������ռ���Ľ���
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

    // ���ڼ����ַ�
    QString getWeekDayStr()
    {
        QString str = cn("����");
        return str.append(weekStr[curDate.dayOfWeek() - 1]);
    }

    // 28���޼���
    QString getChineseConstellation()
    {
        int offset = 0;
        int modStarDay = 0;
        offset = ChineseConstellationReferDay.daysTo(curDate);
        modStarDay = offset % 28;
        return modStarDay >= 0 ? ChineseConstellationName[modStarDay] :
            ChineseConstellationName[27 + modStarDay];
    }

    // �Ƿ�����
    bool isChineseLeapMonth()
    {
        return isLeapMonth;
    }

    // �����Ƿ�������
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
            // ����
            if ((leap > 0) && (i == leap + 1) && (isLeapMonth == false))
            {
                isLeapMonth = true;
                i = i -1;
                temp = getChineseLeapMonthDays(year); //������������
            }
            else
            {
                isLeapMonth = false;
                temp = getChineseMonthDays(year, i); //�������������
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
        sumDay = 348; //29�� X 12����
        i = 0x8000;
        info = LunarDateArray[year - minYear] & 0x0FFFF;
        //0x04BD8  & 0x0FFFF �м�12λ����4BD��ÿλ����һ���£�Ϊ1��Ϊ���£�Ϊ0��ΪС��
        //����12�������ж�����Ϊ30��
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
        if (date.month() == month) //�·���ͬ
        {
            if (date.dayOfWeek() == day) //���ڼ���ͬ
            {
                QDate firstDay(date.year(), date.month(), 1);//���ɵ��µ�һ��
                int i = firstDay.dayOfWeek();
                int firWeekDays = 7 - firstDay.dayOfWeek() + 1; //�����һ��ʣ������
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
    int month; // ũ���·�
    int day;    // ũ��������
    bool isLeapMonth; // �����Ƿ�����
    bool isLeapYear;    // �����Ƿ�����
};

#endif // LUNAR
