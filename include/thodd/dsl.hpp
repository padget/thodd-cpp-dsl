#ifndef __THODD_DSL_DSL_HPP__
#  define  __THODD_DSL_DSL_HPP__ 

#  include <tuple>
#  include <type_traits>
#  include <thodd/functional.hpp>

namespace
thodd   
{    
    template<
        typename id_t, id_t nodeid_c,
        typename act_t>
    struct node 
    {
        act_t act ;
    } ; 


    template<
        typename id_t, id_t id_c>
    constexpr auto
    as_node(
        std::integral_constant<id_t, id_c>,
        auto&& __act)
    {
        return
        node<id_t, id_c, std::decay_t<decltype(__act)>>
        { static_cast<decltype(__act)&&>(__act) } ;
    }


    template<
        typename interpret_provider_t, 
        typename ... nodes_t>
    struct dsl 
    {
        std::tuple<nodes_t...> expression ;

        constexpr auto
        operator () (
            auto && ... __params) const 
        {
            return 
            std::apply(
                [&] (auto&& ... __nodes) 
                { 
                    return 
                    interpret_provider_t{}
                        .get_interpretor(static_cast<decltype(__nodes)&&>(__nodes)...) (
                            static_cast<decltype(__params)&&>(__params)...) ; 
                },
                expression);
        }
    } ;   



    template<
        typename interpret_provider_t,
        typename ... nodes_t, 
        typename id_t, id_t id_c, 
        typename act_t>
    constexpr auto
    operator << (
        dsl<interpret_provider_t, nodes_t...> const& __dsl, 
        node<id_t, id_c, act_t> const& __node)
    {
        return 
        dsl<
            interpret_provider_t, 
            nodes_t..., node<id_t, id_c, act_t>>
        { std::tuple_cat(
            __dsl.expression, 
            std::tuple(__node))  } ;
    }       
}

#endif