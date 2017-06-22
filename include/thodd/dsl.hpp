#ifndef __THODD_DSL_DSL_HPP__
#  define  __THODD_DSL_DSL_HPP__ 

#  include <tuple>
#  include <type_traits>
#  include <thodd/sequence.hpp>

namespace
thodd
{
    template<
        typename ... nodes_t>
    struct dsl_expression
    {
        std::tuple<nodes_t...> nodes ;    
    } ;


    template<
        typename node_t>
    struct dsl_node
    {
        node_t node ;
    } ;


    template<
        typename ... expressions_t>
    struct dsl_expressions
    {
        std::tuple<expressions_t...> expressions ;
    } ;


    template<
        size_t index_c,
        typename ... nodes_t>
    constexpr auto
    get(
        dsl_expression<nodes_t...> const& __cdsl)
    {
        return 
        std::get<index_c>(
            __cdsl.nodes) ;
    }


    template<
        size_t index_c,
        typename ... expressions_t>
    constexpr auto
    get(
        dsl_expressions<expressions_t...> const& __cdsl)
    {
        return 
        std::get<index_c>(
            __cdsl.expressions) ;
    }


    template<
        typename ... nodes_t,
        typename node_t>
    constexpr auto
    operator > (
        dsl_expression<nodes_t...> const& __cdsl,
        dsl_node<node_t> const& __node)
    {
        return 
        dsl_expression<nodes_t..., std::decay_t<node_t>>
        { std::tuple_cat(
            __cdsl.nodes, 
            std::make_tuple(__node.node)) } ;
    }


    template<
        typename ... nodes_t,
        typename node_t>
    constexpr auto
    operator > (
        dsl_node<node_t> const& __node, 
        dsl_expression<nodes_t...> const& __cdsl)
    {
        return 
        dsl_expression<std::decay_t<node_t>, nodes_t...>
        { std::tuple_cat(
            std::make_tuple(__node.node), 
            __cdsl.nodes) } ;
    }


    template<
        typename func1_t, 
        typename func2_t>
    constexpr auto
    operator > (
        dsl_node<func1_t> const& __node1,
        dsl_node<func2_t> const& __node2)
    {
        return 
        dsl_expression<func1_t, func2_t>
        { std::make_tuple(__node1.node, __node2.node) } ;
    }


    template<
        typename ... lnodes_t, 
        typename ... rnodes_t>
    constexpr auto 
    operator , (
        dsl_expression<lnodes_t...> const& __lexpression,
        dsl_expression<rnodes_t...> const& __rexpression)
    {
        using left_t = dsl_expression<lnodes_t...> ;
        using right_t = dsl_expression<rnodes_t...> ;

        return 
        dsl_expressions<left_t, right_t>
        { std::make_tuple(
            __lexpression, 
            __rexpression) } ;
    }


    template<
        typename ... expressions_t,
        typename ... nodes_t>
    constexpr auto
    operator , (
        dsl_expressions<expressions_t...> const& __expressions,
        dsl_expression<nodes_t...> const __expression)
    {
        using right_t = dsl_expression<nodes_t...> ;

        return 
        dsl_expressions<expressions_t..., right_t>
        { std::tuple_cat(
            __expressions.expressions, 
            std::make_tuple(__expression) } ;
    }

    
    template<
        template<typename...> typename node_t>
    constexpr auto
    make_node(
        auto&&... __funcs)
    {
        return 
        dsl_node<node_t<
            std::::decay_t<decltype(__funcs)>...>>
        { { static_cast<auto&&>(__funcs)... } } ; 
    }


    namespace detail
    {
        template<
            template<typename> 
            typename dsl_t>
        struct go_launcher
        {
             template<
                typename ... nodes_t>
            constexpr auto 
            operator()(
                dsl_expression<nodes_t...> const& __dsl, 
                auto&&... __args) const
            {
                constexpr dsl_t<dsl_expression<nodes_t...>> __lang{} ;

                return 
                __lang(
                    __dsl, 
                    static_cast<decltype(__args)&&>(__args)...) ;
            }


            template<
                typename ... expressions_t, 
                size_t ... indexes_c>
            constexpr void 
            operator()(
                sequence<size_t, indexes_c...> const&, 
                dsl_expressions<expressions_t...> const& __dsls,
                auto&&... __args) const
            {
                expand{ (go_launcher<dsl_t>{}(get<indexes_c>(__dsls), 
                        static_cast<decltype(__args)&&>(__args)...), 0)... } ;
            }


            template<
                typename ... expressions_t>
            constexpr void 
            operator()(
                dsl_expressions<expressions_t...> const& __dsls,
                auto&&... __args) const
            {
                go_launcher<dsl_t>{}(
                    make_indexes<sizeof...(expressions_t)>{}, 
                    __dsls, 
                    static_cast<decltype(__args)&&>(__args)...) ;
            }
        } ;
    }


    template<
        template<typename>
        typename dsl_t>
    extern constexpr auto go = detail::go_launcher<dsl_t>{} ;
}

#endif