#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <stack>
#include <functional>
#include <sstream>
#include <random>
#include <optional>

#include "tokens.hh"
#include "rpn.hh"
#include "utils.hh"

template <typename T>
struct BinaryOperator {
    char operation_symbol;
    std::function<T(T,T)> operation;
    int precedence;
    BinaryOperator() = default;
    BinaryOperator(char operation_symbol, std::function<T(T,T)> operation, int precedence):
            operation_symbol(operation_symbol), operation(operation), precedence(precedence) {};
    // BinaryOperator(const BinaryOperator & other) = default;
    // BinaryOperator & operator=(const BinaryOperator & other) = default;
};

int random_number(int x, int y);

int roll_and_add(int times, int sides);

template <typename LiteralType>
class Node {
    using NodePtr = std::unique_ptr<Node>;
public:
    using NodeType = std::optional<BinaryOperator<LiteralType>>;
private:
    NodeType node_type;
    LiteralType value;
    NodePtr left, right;
public:
    explicit Node(LiteralType value) : value(value) {};
    Node(NodeType node_type, NodePtr & left, NodePtr & right) :
            node_type(node_type), left(std::move(left)), right(std::move(right)) {};

    LiteralType evaluate() const {
        if (!node_type) {
            return value;
        }
        else {
            auto left_value = left->evaluate();
            auto right_value = right->evaluate();
            return node_type->operation(left_value, right_value);
        }
    }

    friend
    std::ostream & operator<<(std::ostream & out, const Node & node) {
        if (!node.node_type) {
            out << node.value;
        }
        else {
            if (bracket_condition(node.node_type, node.left -> node_type)) {
                out << "(" << *node.left << ")";
            }
            else {
                out << *node.left;
            }
            out << " " << node.node_type->operation_symbol << " ";
            if (bracket_condition(node.node_type, node.right -> node_type)) {
                out << "(" << *node.right << ")";
            }
            else {
                out << *node.right;
            }
        }
        return out;
    };
};

template <typename T>
bool bracket_condition(const typename Node<T>::NodeType & parent_type, const typename  Node<T>::NodeType & child_type) {
    if (!child_type)
        return false;
    return parent_type->precedence > child_type->precedence;
}


template<typename T>
T extract(std::stack<T> & stack) {
    T top = std::move(stack.top());
    stack.pop();
    return top;
}


template <typename T>
class NodeBuilder {
    std::map<std::string, BinaryOperator<T>> token_types_map;
    std::vector<char> op_symbols;
public:
    explicit NodeBuilder(const std::vector<BinaryOperator<T>> & ops) {
        for (const auto & op: ops) {
            token_types_map[std::string(1, op.operation_symbol)] = op;
            op_symbols.push_back(op.operation_symbol);
        }
    };

    std::unique_ptr<Node<T>> to_node(const std::vector<Token> & token_list) {
        std::stack<std::unique_ptr<Node<T>>> node_stack;
        for (const Token & token : token_list) {
            if (token.token_type == TOKEN_LITERAL) {
                auto value = from_str<T>(token.value);
                if (!value) return nullptr;
                node_stack.push(std::make_unique<Node<T>> (*value));
            }
            else if (token.token_type == OPERATOR) {
                if (node_stack.size() < 2) {
                    return nullptr;
                }
                auto right = extract(node_stack);
                auto left = extract(node_stack);
                node_stack.push(std::make_unique<Node<T>> (
                        token_types_map[token.value], left, right));
            }
        }
        if (node_stack.size() != 1) {
            return nullptr;
        }
        return extract(node_stack);
    }

    std::unique_ptr<Node<T>> string_to_node(const std::string & str) {
        auto token_list = tokenize(str, op_symbols);
        if (!token_list.has_value()) {
            std::cout << "miau0";
            return nullptr;
        }
        auto rpned = to_rpn(*token_list);
        if (!rpned.has_value()) {
            std::cout << "miau1";
            return nullptr;
        }
        return to_node(*rpned);
    }

};
