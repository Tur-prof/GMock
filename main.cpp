#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream> 
#include <string>

class DBconnectionInterface
{
public:
    DBconnectionInterface() {};
    virtual ~DBconnectionInterface() {};
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool execQuery(const std::string& query) = 0;
};

class DBConnection : virtual public DBconnectionInterface
{
public:
    DBConnection() {};
    virtual ~DBConnection() {};
    virtual bool open()
    {
        return true;
    }
    virtual bool close()
    {
        return true;
    }
    virtual bool execQuery(const std::string& query)
    {
        return true;
    }
};

class ClassThatUsesDB
{
private:
    DBconnectionInterface* _db;

public:

    ClassThatUsesDB(DBconnectionInterface* connect) : _db(connect)
    {
    }

    bool openConnection()
    {
        return _db->open();
    }

    bool closeConnection()
    {
        return _db->close();
    }
    bool useConnection(const std::string& query)
    {
        return _db->execQuery(query);
    }
};

class DBConnectionMock : virtual public DBconnectionInterface
{
public:
    MOCK_METHOD(bool, open, (), (override));
    MOCK_METHOD(bool, close, (), (override));
    MOCK_METHOD(bool, execQuery, (const std::string& query), (override));
};

class MyTest : public ::testing::Test
{
protected:
    void SetUp()
    {
        _test_db = new DBConnection();
        _test_uses = new ClassThatUsesDB(_test_db);
    }

    void TearDown()
    {
        delete _test_db;
        delete _test_uses;
    }
protected:
    DBconnectionInterface* _test_db;
    ClassThatUsesDB* _test_uses;
};

TEST_F(MyTest, testcase1)
{
    bool test = _test_uses->openConnection();
    ASSERT_EQ(test, true);
}

TEST_F(MyTest, testcase2)
{
    bool test = _test_uses->useConnection("OK");
    ASSERT_EQ(test, true);
}

TEST_F(MyTest, testcase3)
{
    bool test = _test_uses->closeConnection();
    ASSERT_EQ(test, true);
}

TEST_F(MyTest, testcase4)
{
    DBConnectionMock gmock;

    EXPECT_CALL(gmock, open).WillOnce(::testing::Return(true));
    EXPECT_CALL(gmock, execQuery("OK (GMock)")).WillOnce(::testing::Return(true));
    EXPECT_CALL(gmock, close).WillOnce(::testing::Return(true));

    ClassThatUsesDB _db_uses(&gmock);
    bool test_1 = _db_uses.openConnection();
    bool test_2 = _db_uses.useConnection("OK (GMock)");
    bool test_3 = _db_uses.closeConnection();

    ASSERT_EQ(test_1, true);
    ASSERT_EQ(test_2, true);
    ASSERT_EQ(test_3, true);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}