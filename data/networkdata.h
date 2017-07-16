#ifndef NETWORKDATA_H
#define NETWORKDATA_H


class NetworkData
{
public:
    NetworkData();
    NetworkData(const NetworkData &data);
    NetworkData(char *data, int length);
    virtual~NetworkData();
    char *data();
    int length();
    NetworkData *operator=(const NetworkData &data);
    void setData(char *data, int length);
    void appendData(char *data, int length);
    void clear();
    bool isEmpty();
private:
    class PrivData;
    PrivData *mData;
};

#endif // NETWORKDATA_H
