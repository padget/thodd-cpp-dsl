#ifndef __THODD_DSL_DSL_HPP__
#  define  __THODD_DSL_DSL_HPP__ 

#  include <tuple>
#  include <type_traits>
#  include <thodd/sequence.hpp>

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
        typename id_t,
        id_t id_c>
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
        typename ... nodes_t>
    struct expression
    {
        std::tuple<nodes_t...> nodes ;    
    } ;

    
    as_expression(
        auto&& ... __expr)
    {
        return 
        expression<std::decay_t<decltype(__expr)>...>
        { std::make_tuple(static_cast<decltype(__expr)&&>(__expr)...) } ;
    }


    template<
        typename id_t, id_t id1_c, id_t id2_c, 
        typename act1_t, typename act2_t>
    constexpr auto
    operator > (
        node<id_t, id1_c, act1_t> const& __node1,
        node<id_t, id2_c, act2_t> const& __node2)
    {
        return 
        expression<
            node<id_t, id1_c, act1_t>, 
            node<id_t, id2_c, act2_t>>
        { std::make_tuple(__node1, __node2) } ;
    }

   template<
        typename id_t, id_t id_c, typename act_t, 
        typename ... nodes_t>
    constexpr auto 
    operator > (
        expression<nodes_t...> const& __expr,
        node<id_t, id_c, act_t> const& __node)
    {
        return 
        expression<nodes_t..., node<id_t, id_c, act_t>>
        { std::tuple_cat(
            __expr.nodes, 
            std::make_tuple(__node)) } ;
    }

    
    template<
        typename id_t, id_t id_c, typename act_t, 
        typename ... nodes_t>
    constexpr auto 
    operator > (
        node<id_t, id_c, act_t> const& __node, 
        expression<nodes_t...> const& __expr)
    {
        return 
        expression<nodes_t..., node<id_t, id_c, act_t>>
        { std::tuple_cat(
            std::make_tuple(__node), 
            __expr.nodes) } ;
    }
    

    template<typename ... nodes_t>
    extern constexpr auto go = [] (expression<nodes_t...> const& __expr, auto&&... __args)
        {
            return 2;
        } ;


    // template<
    //     typename ... expressions_t>
    // struct dsl_expressions
    // {
    //     std::tuple<expressions_t...> expressions ;
    // } ;



    // template<
    //     typename ... lnodes_t, 
    //     typename ... rnodes_t>
    // constexpr auto 
    // operator , (
    //     dsl_expression<lnodes_t...> const& __lexpression,
    //     dsl_expression<rnodes_t...> const& __rexpression)
    // {
    //     using left_t = dsl_expression<lnodes_t...> ;
    //     using right_t = dsl_expression<rnodes_t...> ;

    //     return 
    //     dsl_expressions<left_t, right_t>
    //     { std::make_tuple(
    //         __lexpression, 
    //         __rexpression) } ;
    // }


    // template<
    //     typename ... expressions_t,
    //     typename ... nodes_t>
    // constexpr auto
    // operator , (
    //     dsl_expressions<expressions_t...> const& __expressions,
    //     dsl_expression<nodes_t...> const& __expression)
    // {
    //     using right_t = dsl_expression<nodes_t...> ;

    //     return 
    //     dsl_expressions<expressions_t..., right_t>
    //     { std::tuple_cat(
    //         __expressions.expressions, 
    //         std::make_tuple(__expression)) } ;
    // }


   
}

#endif