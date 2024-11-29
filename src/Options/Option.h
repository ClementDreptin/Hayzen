#pragma once

class Option
{
public:
    typedef std::function<bool(void *)> Callback;

    Option();

    virtual ~Option() {}

    Option(const std::string &name, Callback callback);

    inline const std::string &GetName() const { return m_Name; }

    void Select(bool selected) { m_IsSelected = selected; }

    virtual bool Update(Input::Gamepad *pGamepad) = 0;

    virtual void Render(float x, float y, float width) = 0;

    virtual float GetMinWidth() const;

    float GetMinHeight() const;

protected:
    std::string m_Name;
    Callback m_Callback;
    bool m_IsSelected;

    mutable float m_CachedMinWidth;
    mutable float m_CachedMinHeight;
};

// Macro to create a shared pointer to an option. Being able to use templated arguments and std::forward
// would be much nicer but they were never implemented for the Xbox 360 compiler.
#define MakeOption(OptionType, ...) std::make_shared<OptionType>(__VA_ARGS__)
