/*
  ==============================================================================

    ColorSourceOverrideEffect.h
    Created: 9 Nov 2020 12:54:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSource;

class ColorSourceOverrideEffect :
    public ColorEffect,
    public ColorSource::ColorSourceListener
{
public:
    ColorSourceOverrideEffect(var params);
    ~ColorSourceOverrideEffect();

    std::unique_ptr<ColorSource> colorSource;
    ColorSource* templateRef;

    void setupSource(const String& type, ColorSource * templateRef = nullptr);

    void processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time = -1);

    virtual void colorSourceParamControlModeChanged(Parameter* p) override;

    var getJSONData(bool includeNonOverriden = false) override;
    void loadJSONDataItemInternal(var data) override;

    class OverrideEffectEvent
    {
    public:
        enum Type { SOURCE_CHANGED };

        OverrideEffectEvent(Type t, ColorSourceOverrideEffect* ef) : type(t), effect(ef) {}
        Type type;
        ColorSourceOverrideEffect * effect;
    };

    QueuedNotifier<OverrideEffectEvent> overrideEffectNotifier;
    typedef QueuedNotifier<OverrideEffectEvent>::Listener AsyncOverrideEffectListener;

    void addAsyncOverrideEffectListener(AsyncOverrideEffectListener* newListener) { overrideEffectNotifier.addListener(newListener); }
    void addAsyncCoalescedOverrideEffectListener(AsyncOverrideEffectListener* newListener) { overrideEffectNotifier.addAsyncCoalescedListener(newListener); }
    void removeAsyncOverrideEffectListener(AsyncOverrideEffectListener* listener) { overrideEffectNotifier.removeListener(listener); }


    String getTypeString() const override { return getTypeStringStatic(); }
    const static String getTypeStringStatic() { return "Override (Color)"; }
    static ColorSourceOverrideEffect* create(var params) { return new ColorSourceOverrideEffect(params); }

    InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
};