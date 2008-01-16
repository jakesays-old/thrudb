
#ifdef HAVE_CONFIG_H
#include "thrudoc_config.h"
#endif
/* hack to work around thrift and log4cxx installing config.h's */
#undef HAVE_CONFIG_H

#include "ThrudocBackend.h"

using namespace thrudoc;
using namespace std;

vector<ThrudocException> ThrudocBackend::putList 
(const vector<Element> & elements)
{
    vector<ThrudocException> exceptions;
    ThrudocException none;
    vector<Element> e = (vector<Element>)elements;
    vector<Element>::iterator i;
    for (i = e.begin (); i != e.end (); i++)
    {
        try
        {
            put ((*i).tablename, (*i).key, (*i).value);
            exceptions.push_back (none);
        }
        catch (ThrudocException e)
        {
            exceptions.push_back (e);
        }
    }
    return exceptions;
}

vector<ListResponse> ThrudocBackend::getList 
(const vector<Element> & elements)
{
    vector<ListResponse> list_responses;
    vector<Element> e = (vector<Element>)elements;
    vector<Element>::iterator i;
    for (i = e.begin (); i != e.end (); i++)
    {
        ListResponse list_response;
        try
        {
            list_response.element.tablename = (*i).tablename;
            list_response.element.key = (*i).key;
            list_response.element.value = get ((*i).tablename, (*i).key);
        }
        catch (ThrudocException e)
        {
            list_response.ex = e;
        }
        list_responses.push_back (list_response);
    }
    return list_responses;
}

vector<ThrudocException> ThrudocBackend::removeList 
(const vector<Element> & elements)
{
    vector<ThrudocException> exceptions;
    ThrudocException none;
    vector<Element> e = (vector<Element>)elements;
    vector<Element>::iterator i;
    for (i = e.begin (); i != e.end (); i++)
    {
        try
        {
            remove ((*i).tablename, (*i).key);
            exceptions.push_back (none);
        }
        catch (ThrudocException e)
        {
            exceptions.push_back (e);
        }
    }
    return exceptions;
}

void ThrudocBackend::validate (const std::string & tablename,
                               const std::string * key,
                               const std::string * value)
{
    if (tablename.empty () || (tablename.find (" ") !=
                               std::string::npos))
    {
        thrudoc::ThrudocException e;
        e.what = "invalid tablename";
        throw e;
    }
    else if (key && (*key) == "")
    {
        ThrudocException e;
        e.what = "invalid key";
        throw e;
    }
}