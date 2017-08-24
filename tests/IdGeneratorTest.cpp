#define BOOST_TEST_MODULE ID_GENERATOR
#include <boost/test/unit_test.hpp>
#include "../include/IdGenerator.h"

#include <thread>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <mutex>

using namespace IdGenerator;
using namespace std;

int ITERATION_COUNT = 1000;
int THREAD_COUNT = 100;

mutex boostMutex;

BOOST_AUTO_TEST_CASE (sample)
{
    auto generator1 = GeneratorBuilder<int>::build ();
    auto generator2 = GeneratorBuilder<long>::build ();
    auto generator3 = GeneratorBuilder<int>::build ();
    auto generator4 = GeneratorBuilder<long>::build ();
    
    int el1 = generator1->generate ();
    int el2 = generator3->generate () - 1;
    long lel1 = generator2->generate ();
    long lel2 = generator2->generate () - 1;
    BOOST_CHECK_EQUAL (el1, el2);
    BOOST_CHECK_EQUAL (lel1, lel2);
    
}

BOOST_AUTO_TEST_CASE (different_settings)
{
    auto generator1 = GeneratorBuilder<int>::build ();
    generator1->generate ();
    auto generator2 = GeneratorBuilder<int>::build ();
    auto generator3 = GeneratorBuilder<int>::build (true);
    BOOST_CHECK (generator2->generate () != generator3->generate ());
}


void threadCheck ()
{
    auto generator = GeneratorBuilder<int>::build ();
    int index = generator->generate ();
    for (int i = 0;i < ITERATION_COUNT;++i)
    {
        int newIndex = generator->generate ();
        {
            lock_guard<mutex> locker (boostMutex); // BOOST_CHECK is not thread safe
            BOOST_CHECK (index < newIndex);
        }
        index = newIndex;
    }
}

BOOST_AUTO_TEST_CASE (threads_safe)
{
    vector<shared_ptr<thread>> threads;
    auto generator = GeneratorBuilder<int>::build ();
    int indexBefore = generator->generate ();
    for (int i = 0;i < THREAD_COUNT;++i)
    {
        auto thr = make_shared<thread> (threadCheck);
        threads.push_back (thr);
    }
    
    for_each (threads.begin (), threads.end (), [](auto thr){
        thr->join ();
    });
    
    int assumedIndex = ITERATION_COUNT * THREAD_COUNT + THREAD_COUNT + indexBefore + 1;
    auto realIndex = generator->generate ();
    BOOST_CHECK_EQUAL (realIndex,assumedIndex);
    
}

BOOST_AUTO_TEST_CASE (limit_check)
{
    auto generator = GeneratorBuilder<int>::build ();
    string exMsg;
    try
    {
        generator->generate ();
        for (int i = 0;i < numeric_limits<int>::max ();i++)
        {
            generator->generate ();
        }
    }
    catch (const exception& ex)
    {
        exMsg = string (ex.what ());
    }
    
    OutOfLimitException ex;
    BOOST_CHECK (string (ex.what ()) == exMsg);
    
    
}


