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

    
    constexpr auto
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
    
    template<
        typename interpret_t>
    struct go
    {
        template<
            typename ids_t,
            ids_t ... ids_c,
            typename ... acts_t>
        inline auto
        operator () (
            expression<node<ids_t, ids_c, acts_t>...> const& __expr) 
        {
        
            return 
            (*this)(
                reverse_sequence(
                    make_sequence(
                        std::integral_constant<size_t, sizeof...(acts_t) - 1>{})), 
                __expr) ;
        }

    private:
        template<
            typename ids_t,
            ids_t ... ids_c,
            typename ... acts_t, 
            size_t ... idxs_c>
        inline auto
        operator () (
            sequence<size_t, idxs_c...> const&,
            expression<node<ids_t, ids_c, acts_t>...> const& __expr) 
        {
            return 
            interpret_t{}.interpret(std::get<idxs_c>(__expr.nodes)...) ;
        }
        
    };   
}

#endif