#include<iomanip>
#include "parser.h"
#include "error.h"

Node Parser::statement()
{
    Token t = ts_.next();
    if (t.getType() == T_KEYWORD) {
        switch (t.getId())
        {
        case '{': return compound_stmt();
        case K_IF: return if_stmt();
        case K_FOR:return for_stmt();
        case K_WHILE:return while_stmt();
        case K_DO:return do_stmt();
        case K_RETURN: return return_stmt();
        case K_SWITCH: return switch_stmt();
        case K_CASE: return case_stmt();
        case K_DEFAULT: return default_stmt();
        case K_BREAK: return break_stmt();
        case K_CONTINUE:return continue_stmt();
        case K_GOTO: return goto_stmt();
        }
    }

    if (t.getType() == T_IDENTIFIER && next_is(':')) {
        // label
        labels.push_back(t.getSval());
        out << t.getSval() << ":" << std::endl;
        return Node();
    }
    ts_.back();

    Node r = expr_opt();
    expect(';');

    return r;
}

Node Parser::compound_stmt()
{
    __IN_SCOPE__(localenv, localenv, newLabel("Env"));

    std::vector<Node> list;

    for (;;) {
        if (next_is('}')) break;
        decl_or_stmt(list);
    }

    __OUT_SCOPE__(localenv);
    return createCompoundStmtNode(list);
}

void Parser::decl_or_stmt(std::vector<Node> &list)
{
    if (ts_.peek().getId() == T_EOF)
        error("premature end of input");

    if (is_type(ts_.peek())) {
        declaration(list, false);
    }
    else {
        list.push_back(statement());
    }
    out << "clr" << std::endl;
}

Node Parser::if_stmt()
{
    BoolLabel _if;
    std::string snext = newLabel("sn");

    expect('(');
    _EN_CONDITION_();
    Node *cond = new Node(bool_expr());          // B.code
    _DIS_CONDITION_();
    expect(')');

    _if._true = newLabel("ift");
    _if._false = newLabel("iff");
    boolLabel.back()._true = _if._true;
    boolLabel.back()._false = _if._false;
    generateIfGoto();
    out << _if._true << ":" << std::endl;

    out << "clr" << std::endl;
    Node *then = new Node(statement());     // S1.code

    if (next_is(K_ELSE)) {
        out << "goto\t" << snext << std::endl;
        out << _if._false << ":" << std::endl;
        Node *els = new Node(statement());
        out << snext << ":" << std::endl;
        return createIfStmtNode(cond, then, els);
    }
    else {
        out << _if._false << ":" << std::endl;          // S1.next
    }
    return createIfStmtNode(cond, then, nullptr);
}

Node Parser::while_stmt()
{
    BoolLabel _while;
    std::string _begin = newLabel("wb");   // begin = newLabel
    std::string _snext = newLabel("sn");

    // break �� continue
    _stk_ctl_bg_l.push_back(_begin);
    _stk_ctl_end_l.push_back(_snext);

    _while._true = newLabel("wt");         // B.true = newLabel
    _while._false = _snext;                // B.false = S.next

    std::string _s1next = _begin;          // S1.next = begin

    out << _begin << ":" << std::endl;     // Label(begin)

    expect('(');
    _EN_CONDITION_();
    Node node = bool_expr();
    _DIS_CONDITION_();
    expect(')');

    boolLabel.back()._true = _while._true;
    boolLabel.back()._false = _while._false;
    generateIfGoto();                      // B.code
    out << _while._true << ":" << std::endl;           // Label(B.true)

    out << "clr" << std::endl;

    Node body = statement();               // S1.code
    out << "goto " << _begin << std::endl; // gen(goto begin)

    out << _snext << ":" << std::endl;

    _stk_ctl_bg_l.pop_back();
    _stk_ctl_end_l.pop_back();

    std::vector<Node> list;
    return createCompoundStmtNode(list);
}


Node Parser::do_stmt()
{
    BoolLabel doLabel;
    std::string begin = newLabel("db");   // begin = newLabel
    std::string snext = newLabel("sn");

    _stk_ctl_bg_l.push_back(begin);
    _stk_ctl_end_l.push_back(snext);

    doLabel._true = begin;
    doLabel._false = snext;

    out << begin << ":" << std::endl;
    Node *r = new Node(statement());
    expect(K_WHILE);
    expect('(');
    _EN_CONDITION_();
    Node *_b = new Node(bool_expr());
    _DIS_CONDITION_();
    expect(')');
    expect(';');

    boolLabel.back()._true = doLabel._true;
    boolLabel.back()._false = doLabel._false;
    generateIfGoto();                      // B.code

    out << snext << ":" << std::endl;

    _stk_ctl_bg_l.pop_back();
    _stk_ctl_end_l.pop_back();

    return *r; ////////////////////����Ҫ�޸�
}

Node Parser::switch_stmt()
{
    std::string _next = newLabel("swn");

    _stk_ctl_end_l.push_back(_next);

    expect('(');
    Node r = expr();
    if (r.type.getType() != K_INT && r.type.getType() != K_CHAR
        && r.type.getType() != K_SHORT && r.type.getType() != K_LONG)
        error("Switch only integer!");

    if (r.kind == NODE_INT || r.kind == NODE_CHAR || r.kind == NODE_SHORT || r.kind == NODE_LONG)
        switch_expr = std::to_string(r.int_val);
    else if (r.kind == NODE_LOC_VAR || NODE_GLO_VAR)
        switch_expr = r.name();
    else
        switch_expr = _stk_quad.back();
    expect(')');

    expect('{');
    compound_stmt();

    out << _next << ":" << std::endl;
    _stk_ctl_end_l.pop_back();
    return Node();
}

Node Parser::for_stmt()
{
    std::string _next = newLabel("forn");
    std::string _begin = newLabel("forb");
    std::string _exp3 = newLabel("fe3");


    BoolLabel _for;

    _stk_ctl_bg_l.push_back(_begin);
    _stk_ctl_end_l.push_back(_next);

    _for._false = _next;
    _for._true = newLabel("fort");

    expect('(');
    __IN_SCOPE__(localenv, localenv, newLabel("for"));
    if (is_type(ts_.peek())) {
        std::vector<Node> list;
        declaration(list, false);
    }
    else if (is_keyword(ts_.peek(), ';')) {
        expect(';');
    }
    else {
        expr();
        expect(';');
    }

    out << _begin << ":" << std::endl;

    if (is_keyword(ts_.peek(), ';')) {
        expect(';');
    }
    else {
        _EN_CONDITION_();
        bool_expr();
        _DIS_CONDITION_();
        expect(';');
    }

    boolLabel.push_back(_for);
    generateIfGoto();
    out << "clr" << std::endl;

    out << _exp3 << ":" << std::endl;
    if (is_keyword(ts_.peek(), ')')) {
        expect(')');
    }
    else {
        expr();
        expect(')');
    }
    out << "clr" << std::endl;

    out << "goto " << _begin << std::endl;
    out << _for._true << ":" << std::endl;

    statement();
    __OUT_SCOPE__(localenv);
    out << "goto " << _exp3 << std::endl;
    out << _next << ":" << std::endl;

    _stk_ctl_bg_l.pop_back();
    _stk_ctl_end_l.pop_back();

    return Node();
}


Node Parser::goto_stmt()
{
    Token t = ts_.next();
    labels.push_back_un(t.getSval());
    expect(';');

    out << std::left << std::setw(10) << "goto " << t.getSval() << std::endl;

    return Node();
}

Node Parser::continue_stmt()
{
    out << "goto " << _stk_ctl_bg_l.back() << std::endl;
    return  Node();
}

Node Parser::return_stmt()
{
    Node *retval = new Node(expr_opt());
    expect(';');

    out << "ret\t" + _stk_quad.back() << std::endl;

    return createRetStmtNode(retval);
}


Node Parser::case_stmt()
{
    int val = com_conditional_expr().int_val;
    expect(':');

    out << "if " << switch_expr << " != " << val << " goto " << switch_case_label << std::endl;
    statement();

    out << switch_case_label << ":" << std::endl;
    switch_case_label = newLabel("case");

    return Node();
}
Node Parser::default_stmt()
{
    expect(':');
    statement();
    return Node();
}

Node Parser::break_stmt()
{
    out << "goto " << _stk_ctl_end_l.back() << std::endl;
    expect(';');
    return createJumpNode(label_break);
}