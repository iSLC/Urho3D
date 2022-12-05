//
// Copyright (c) 2008-2022 the Urho3D project.
//

#pragma once

#include "../Base/Utility.h"

namespace Urho3D {

/// Forward declaration of the callback forwarder for invalid handlers.
template < class > class CallbackForwarder;

/// Specialization of the callback forwarder type for handlers that don't return a value.
template < class... Args > struct CallbackForwarder< void(Args...) >
{
    /// Signature of the object that forwards the call.
    using Type = void (*)(void *, Args...);

    /// Generate a forwarder for a function handler.
    template < void(*Fptr)(Args...) > [[nodiscard]] static constexpr Type Global()
    {
        return +[](void * obj [[maybe_unused]], Args... args) -> void
        {
            (*Fptr)(Forward< Args >(args)...);
        };
    }

    /// Generate a forwarder for a member handler.
    template < class T, void(T::*Mptr)(Args...) > [[nodiscard]] static constexpr Type Member()
    {
        return +[](void * obj, Args... args) -> void
        {
            (reinterpret_cast< T * >(obj)->*Mptr)(Forward< Args >(args)...);
        };
    }
    /// Generate a forwarder for a constant member handler.
    template < class T, void(T::*Mptr)(Args...) const > [[nodiscard]] static constexpr Type Member()
    {
        return +[](void * obj, Args... args) -> void
        {
            (reinterpret_cast< const T * >(obj)->*Mptr)(Forward< Args >(args)...);
        };
    }

    /// Generate a forwarder for a lambda handler.
    template < class L > [[nodiscard]] static constexpr Type Lambda()
    {
        return +[](void * obj, Args... args) -> void
        {
            (reinterpret_cast< L * >(obj)->operator()(Forward< Args >(args)...));
        };
    }
};

/// Specialization of the callback forwarder type for handlers that return a value.
template < class Ret, class... Args > struct CallbackForwarder< Ret(Args...) >
{
    /// Signature of the object that forwards the call.
    using Type  = Ret (*)(void *, Args...);

    /// Generate an executor for a function handler.
    template < Ret(*Fptr)(Args...) > [[nodiscard]] static constexpr Type Global()
    {
        return +[](void * obj [[maybe_unused]], Args... args) -> Ret
        {
            return (*Fptr)(Forward< Args >(args)...);
        };
    }

    /// Generate an executor for a member handler.
    template < class T, Ret(T::*Mptr)(Args...) > [[nodiscard]] static constexpr Type Member()
    {
        return +[](void * obj, Args... args) -> Ret
        {
            return (reinterpret_cast< T * >(obj)->*Mptr)(Forward< Args >(args)...);
        };
    }
    /// Generate an executor for a constant member handler.
    template < class T, Ret(T::*Mptr)(Args...) const > [[nodiscard]] static constexpr Type Member()
    {
        return +[](void* obj, Args... args) -> Ret
        {
            return (reinterpret_cast< const T * >(obj)->*Mptr)(Forward< Args >(args)...);
        };
    }

    /// Generate an executor for a lambda handler.
    template < class L > [[nodiscard]] static constexpr Type Lambda()
    {
        return +[](void * obj, Args... args) -> Ret
        {
            return (reinterpret_cast< L * >(obj)->operator()(Forward< Args >(args)...));
        };
    }
};

/// Forward declaration of the callback type for invalid callbacks.
template < class > class Callback;

/// Helper structure which forwards an invocation to a callback.
template < class Ret, class... Args > struct Callback< Ret(Args...) >
{
    /// Type of value a callback is expected to return.
    using Return    = Ret;
    /// The signature a callback is expected to have.
    using Signature = Ret(*)(Args...);
    /// Type used to generate the callback forwarders.
    using Forwarder = CallbackForwarder< Ret(Args...) >;

    /// Default constructor.
    constexpr Callback() = default;

    /// Copy constructor.
    constexpr Callback(const Callback & o) = default;
    /// Move constructor.
    constexpr Callback(Callback && o) = default;

    /// Copy assignment operator.
    constexpr Callback & operator = (const Callback & o) = default;
    /// Move assignment operator.
    constexpr Callback & operator = (Callback && o) = default;

    /// Implicit conversion to boolean.
    constexpr operator bool () const { return (exec_ != nullptr); } // NOLINT(google-explicit-constructor)
    /// Implicit conversion to boolean.
    constexpr bool operator ! () const { return !exec_; }

    /// Equality comparison operator.
    constexpr bool operator ==  (const Callback & o) const { return (exec_ == o.exec_) && (this_ == o.this_); }
    /// Inequality comparison operator.
    constexpr bool operator !=  (const Callback & o) const { return (exec_ != o.exec_) || (this_ != o.this_); }

    /// Disable the current callback, if any.
    constexpr void Reset()
    {
        exec_ = nullptr;
        this_ = nullptr;
    }

    /// Use the specified function when forwarding calls.
    template < Ret (*Fptr)(Args...) > [[nodiscard]] static constexpr Callback Global()
    {
        return Callback(Forwarder::template Global< Fptr >(), nullptr);
    }

    /// Use the specified method when forwarding calls.
    template < class T, Ret(T::*Mptr)(Args...) > [[nodiscard]] static constexpr Callback Member(T * obj)
    {
        return Callback(Forwarder::template Member< T, Mptr >(), obj);
    }
    /// Use the specified method when forwarding calls.
    template < class T, Ret(T::*Mptr)(Args...) > [[nodiscard]] static constexpr Callback Member(T & obj)
    {
        return Callback(Forwarder::template Member< T, Mptr >(), AddressOf(obj));
    }

    /// Use the specified constant method when forwarding calls.
    template < class T, Ret (T::*Mptr)(Args...) const > [[nodiscard]] static constexpr Callback Member(const T * obj)
    {
        return Callback(Forwarder::template Member< T, Mptr >(), obj);
    }
    /// Use the specified constant method when forwarding calls.
    template < class T, Ret (T::*Mptr)(Args...) const > [[nodiscard]] static constexpr Callback Member(const T & obj)
    {
        return Callback(Forwarder::template Member< T, Mptr >(), AddressOf(obj));
    }

    /// Use the specified lambda when forwarding calls.
    template < class L > [[nodiscard]] static constexpr Callback Lambda(L * obj)
    {
        return Callback(Forwarder::template Lambda< L >(), obj);
    }
    /// Use the specified lambda when forwarding calls.
    template < class L > [[nodiscard]] static constexpr Callback Lambda(L & obj)
    {
        return Callback(Forwarder::template Lambda< L >(), AddressOf(obj));
    }
protected:

    /// The executor that forwards the callback.
    typename Forwarder::Type exec_{nullptr};
    /// The object that receives the callback (optional).
    void * this_{nullptr};

    /// Construct with the specified executor and environment object.
    constexpr Callback(typename Forwarder::Type fwd, const void * obj)
        : exec_(fwd), this_(const_cast< void * >(obj))
    {
    }

    /// Assign the specified executor and object instance.
    constexpr void Set(typename Forwarder::Type fwd, const void * obj)
    {
        exec_ = fwd;
        this_ = const_cast< void * >(obj);
    }
public:

    /// Forward the call to the assigned callback.
    template < class... Params > constexpr Ret operator() (Params &&... params) const
    {
        // Maybe catch this in debug
        SQ_ASSERT(exec_)
        // Do we have to return something?
        if constexpr (IsVoid_v< Ret >)
        {
            // Is there an executor available?
            if (exec_)
            {
                (*exec_)(this_, Forward< Params >(params)...);
            }
        }
        else
        {
            static_assert(IsDefaultConstructible_v< Ret >);
            // Is there an executor available?
            if (exec_)
            {
                return (*exec_)(this_, Forward< Params >(params)...);
            }
            // Have to return something!
            return Ret{};
        }
    }
};

} // Namespace:: Urho3D
