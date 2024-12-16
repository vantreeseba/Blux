/*
  ==============================================================================

    GroupFilter.cpp
    Created: 7 Oct 2020 4:11:09pm
    Author:  bkupe

  ==============================================================================
*/

GroupFilter::GroupFilter() :
    Filter(getTypeString()),
    groups("Groups")
{
    
    saveAndLoadRecursiveData = false;
    groups.userCanAddControllables = true;
    groups.userAddControllablesFilters.add(TargetParameter::getTypeStringStatic());
    addChildControllableContainer(&groups);

}

GroupFilter::~GroupFilter()
{
    
}

void GroupFilter::controllableAdded(Controllable * c)
{
    
    Filter::controllableAdded(c);
    if (c->parentContainer == &groups)
    {
        if (TargetParameter* tp = dynamic_cast<TargetParameter*>(c))
        {
            tp->setNiceName("Group");
            tp->targetType = TargetParameter::CONTAINER;
            tp->setRootContainer(GroupManager::getInstance());
            tp->maxDefaultSearchLevel = 0;
        }
    }
}

bool GroupFilter::isAffectingObject(Object* o)
{
    for (int i = 0; i < groups.controllables.size(); i++)
    {
        if (TargetParameter* tp = dynamic_cast<TargetParameter*>(groups.controllables[i]))
        {
            if (tp->targetContainer == nullptr) continue;
            if (Group* g = dynamic_cast<Group*>(tp->targetContainer.get()))
            {
                int id = g->getLocalIDForObject(o);
                if (id != -1)  return true;
            }
        }
    }

    return false;
}

FilterResult GroupFilter::getFilteredResultForComponentInternal(Object* o, ObjectComponent* c)
{
    for (int i = 0; i < groups.controllables.size(); i++)
    {
        if (TargetParameter* tp = dynamic_cast<TargetParameter*>(groups.controllables[i]))
        {
            if (tp->targetContainer == nullptr) continue;
            if (Group* g = dynamic_cast<Group*>(tp->targetContainer.get()))
            {
                IDMode m = idMode->getValueDataAsEnum<IDMode>();
                int id = g->getLocalIDForObject(o);
                 
                if (id != -1)
                {
                    if (m == LOCAL_REVERSE) id = g->getNumObjects() - 1 - id;
                    else if (m == RANDOMIZED) id = g->getRandomIDForObject(o);
                    return FilterResult({ id, 1 });
                }
            }
        }
    }
    
    return FilterResult();
}

var GroupFilter::getJSONData(bool includeNonOverriden)
{
    var data = Filter::getJSONData(includeNonOverriden);
    var gData;
    for (auto& c : groups.controllables)
    {
        if (TargetParameter* tp = dynamic_cast<TargetParameter*>(c))
        {
            if (tp->targetContainer == nullptr) continue;
            gData.append(tp->value);
        }
    }
    data.getDynamicObject()->setProperty("groups", gData);
    return data;
}

void GroupFilter::loadJSONDataItemInternal(var data)
{
    Filter::loadJSONDataItemInternal(data);

    var gData = data.getProperty("groups",var());
    for (int i = 0; i < gData.size(); i++)
    {
        TargetParameter* tp = groups.addTargetParameter("Group","");
        tp->setValue(gData[i]);
    }

}
