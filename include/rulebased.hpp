#pragma once

#include "rule.hpp"
#include <iostream>

class RuleBasedExpert {
public:
	int addRule(const std::string &name, ExpressionPtr condition,
		    std::string action)
	{
		Rule rule{nextId, name, std::move(condition),
			  std::move(action)};
		rules.push_back(std::move(rule));
		return nextId++;
	}

	bool removeRule(int id)
	{
		auto it =
		    std::find_if(rules.begin(), rules.end(),
				 [id](const Rule &r) { return r.id == id; });
		if (it == rules.end())
			return false;
		rules.erase(it);
		return true;
	}

	bool removeRuleByName(const std::string &name)
	{
		auto it = std::find_if(
		    rules.begin(), rules.end(),
		    [&name](const Rule &r) { return r.name == name; });
		if (it == rules.end())
			return false;
		rules.erase(it);
		return true;
	}

	void clear() { rules.clear(); }

	Rule *findRule(int id)
	{
		auto it =
		    std::find_if(rules.begin(), rules.end(),
				 [id](const Rule &r) { return r.id == id; });
		return it != rules.end() ? &(*it) : nullptr;
	}

	Rule *findRuleByName(const std::string &name)
	{
		auto it = std::find_if(
		    rules.begin(), rules.end(),
		    [&name](const Rule &r) { return r.name == name; });
		return it != rules.end() ? &(*it) : nullptr;
	}

	size_t size() const { return rules.size(); }
	bool empty() const { return rules.empty(); }

	const std::vector<Rule> &getAllRules() const { return rules; }

	bool updateCondition(int id, ExpressionPtr newCondition)
	{
		Rule *r = findRule(id);
		if (!r)
			return false;
		r->condition = std::move(newCondition);
		return true;
	}

	bool updateAction(int id, std::string newAction)
	{
		Rule *r = findRule(id);
		if (!r)
			return false;
		r->action = std::move(newAction);
		return true;
	}

	void listRules() const
	{
		for (const auto &r : rules)
		{
			std::cout << "[" << r.id << "] " << r.name
				  << " -> action: " << r.action << "\n";
		}
	}

private:
	std::vector<Rule> rules;
	int nextId = 1;
};
