#ifndef __THODD_DSL_DSL_HPP__
#  define  __THODD_DSL_DSL_HPP__ 

#  include <tuple>
#  include <type_traits>
#  include <thodd/sequence.hpp>

namespace
thodd
{
    template<
        typename dslids_t>
    struct dsl
    {
        template<
            dslids_t nodeid_c,
            typename act_t>
        struct node 
        {
            act_t act ;
        } ; 


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
            dslids_t nodeid_c>
        constexpr auto
        as_node(
            std::integral_constant<dslids_t, nodeid_c>,
            auto&& __act)
        {
            return
            node<
                nodeid_c,
                std::decay_t<decltype(__node)>>
            { static_cast<decltype(__node)&&>(__node) } ;
        }
    } ;




    // template<
    //     typename ... expressions_t>
    // struct dsl_expressions
    // {
    //     std::tuple<expressions_t...> expressions ;
    // } ;


    // template<
    //     size_t index_c,
    //     typename ... expressions_t>
    // constexpr auto
    // get (
    //     dsl_expressions<expressions_t...> const& __cdsl)
    // {
    //     return 
    //     std::get<index_c>(__cdsl.expressions) ;
    // }


    template<
        typename dslids_t,
        typename ... nodes_t, 
        typename node_t>
    constexpr auto
    operator > (
        dsl<dslids_t>::expression<nodes_t...> const& __cdsl,
        dsl<dslids_t>::node<node_t> const & __node)
    {
        return 
        dsl<dslids_t>::expression<nodes_t..., node_t>
        { std::tuple_cat(
            __cdsl.nodes, 
            std::make_tuple(__node.node)) } ;
    }


    template<
        typename ... nodes_t, 
        typename node_t>
    constexpr auto
    operator > (
        dsl<dslids_t>::node<node_t> const & __node, 
        dsl<dslids_t>::expression<nodes_t...> const& __cdsl)
    {
        return 
        dsl<dslids_t>::expression<node_t, nodes_t...>
        { std::tuple_cat(
            std::make_tuple(__node.node), 
            __cdsl.nodes) } ;
    }


    template<
        typename lnode_t,
        typename rnode_t>
    constexpr auto
    operator > (
        dsl<dslids_t>::node<lnode_t> const & __node1,
        dsl<dslids_t>::node<rnode_t> const & __node2)
    {
        return 
        dsl<dslids_t>::expression<lnode_t, rnode_t>
        { std::make_tuple(__node1.node, __node2.node) } ;
    }


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
                dsl<dslids_t>::expression<nodes_t...> const& __dsl, 
                auto&&... __args) const
            {
                constexpr dsl_t<dsl<dslids_t>::expression<nodes_t...>> __lang{} ;

                return 
                __lang(
                    __dsl, 
                    static_cast<decltype(__args)&&>(__args)...) ;
            }


            // template<
            //     typename ... expressions_t, 
            //     size_t ... indexes_c>
            // constexpr void 
            // operator()(
            //     sequence<size_t, indexes_c...> const&, 
            //     dsl_expressions<expressions_t...> const& __dsls,
            //     auto&&... __args) const
            // {
            //     (go_launcher<dsl_t>{}(get<indexes_c>(__dsls), static_cast<decltype(__args)&&>(__args)...) , ...) ;
            // }


            // template<
            //     typename ... expressions_t>
            // constexpr void 
            // operator()(
            //     dsl_expressions<expressions_t...> const& __dsls,
            //     auto&&... __args) const
            // {
            //     go_launcher<dsl_t>{}(
            //         make_rsequence_t<size_t, 0, sizeof...(expressions_t) - 1>{}, 
            //         __dsls, 
            //         static_cast<decltype(__args)&&>(__args)...) ;
            // }
        } ;
    }


    template<
        template<typename>
        typename dsl_t>
    extern constexpr auto go = detail::go_launcher<dsl_t>{} ;
}

#endif