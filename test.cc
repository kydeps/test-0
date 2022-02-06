#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <map>

// using Loader = std::function<std::unique_ptr<Expression>(std::istream &)>;
class Expression;
using Loader = std::unique_ptr<Expression>(*)(std::istream &);

class Expression
{
public:
    virtual double compute() = 0;
    virtual ~Expression() = default;
    virtual void PrettyPrint(int indent) = 0;
    virtual std::string PrettyPrint2() = 0;
    virtual void Save(std::ostream &s) = 0;

    static void RegisterLoader(std::string key, Loader loader) {
        loaders_[key] = loader;
    }

    static std::unique_ptr<Expression> LoadFactory(std::istream &s) {
        std::string key;
        s >> key;
        const auto i = loaders_.find(key);
        if (i == loaders_.end())
        {
            std::cerr << "error: found unexpected token " << key << std::endl;
            abort();
        }
        return i->second(s);
    }

private:
    static std::map<std::string, Loader> loaders_;
};

std::map<std::string, Loader> Expression::loaders_ = {};

class Constant final : public Expression
{
public:
    Constant(double x) : x_(x) {
        RegisterLoader("Constant", Load);
    }

    double compute() override { return x_; }

    void PrettyPrint(int indent) override
    {
        std::cout << std::string(indent, ' ') << x_ << std::endl;
    }

    std::string PrettyPrint2() override
    {
        return (std::stringstream() << "(" << x_ << ")").str();
    }

    void Save(std::ostream &s) override
    {
        s << "Constant " << x_ << " ";
    }

    static std::unique_ptr<Expression> Load(std::istream &s)
    {
        double x;
        s >> x;
        return std::make_unique<Constant>(x);
    }

private:
    double x_;
};

class Op final : public Expression
{
public:
    Op(char op, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : op_(op), l_(std::move(l)),
          r_(std::move(r)) 
          {
              RegisterLoader("Op", Load);
          }

    double compute() override
    {
        switch (op_)
        {
        case '^':
            return std::pow(l_->compute(), r_->compute());
        case '+':
            return l_->compute() + r_->compute();
        case '-':
            return l_->compute() - r_->compute();
        case '*':
            return l_->compute() * r_->compute();
        case '/':
            return l_->compute() / r_->compute();
        default:
            abort();
        }
    }

    void PrettyPrint(int indent) override
    {
        std::cout << std::string(indent, ' ') << op_ << std::endl;
        l_->PrettyPrint(indent + 1);
        r_->PrettyPrint(indent + 1);
    }

    std::string PrettyPrint2() override
    {
        return (std::stringstream() << "(" << l_->PrettyPrint2() << op_ << r_->PrettyPrint2() << ")").str();
    }

    void Save(std::ostream &s) override
    {
        s << "Op ";
        s << op_ << " ";
        l_->Save(s);
        r_->Save(s);
    }

    static std::unique_ptr<Expression> Load(std::istream &s)
    {
        char op;
        s >> op;
        auto l = LoadFactory(s);
        auto r = LoadFactory(s);
        return std::make_unique<Op>(op, std::move(l), std::move(r));
    }

private:
    char op_;
    std::unique_ptr<Expression> l_;
    std::unique_ptr<Expression> r_;
};

std::unique_ptr<Expression> parse(std::string t)
{
    for (int i = t.size(); i >= 0; i--)
        if (t[i] == '+' || t[i] == '-')
            return std::make_unique<Op>(t[i], parse(t.substr(0, i)), parse(t.substr(i + 1)));
    for (int i = t.size(); i >= 0; i--)
        if (t[i] == '*' || t[i] == '/')
            return std::make_unique<Op>(t[i], parse(t.substr(0, i)), parse(t.substr(i + 1)));
    for (int i = t.size(); i >= 0; i--)
        if (t[i] == '^')
            return std::make_unique<Op>(t[i], parse(t.substr(0, i)), parse(t.substr(i + 1)));
    return std::make_unique<Constant>(std::stoi(t));
}

int main(int argc, char **argv)
{
    auto t = std::string(argv[1]);
    auto e = parse(t);
    std::cout << e->compute() << std::endl;
    std::cout << e->PrettyPrint2() << std::endl;
    auto ss = std::stringstream();
    e->Save(ss);
    std::cout << ss.str() << std::endl;
    ss.seekg(0);
    auto ee = Expression::LoadFactory(ss);
    std::cout << ee->PrettyPrint2() << std::endl;
    return 0;
}