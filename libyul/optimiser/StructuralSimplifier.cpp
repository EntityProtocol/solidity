/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <libyul/optimiser/StructuralSimplifier.h>
#include <libyul/optimiser/Semantics.h>
#include <libyul/AsmData.h>
#include <libdevcore/CommonData.h>

using namespace std;
using namespace dev;
using namespace yul;

void StructuralSimplifier::operator()(Block& _block)
{
	iterateReplacing(
		_block.statements,
		[&](Statement& _stmt) -> boost::optional<vector<Statement>>
		{
			if (_stmt.type() == typeid(If))
			{
				auto& ifStmt = boost::get<If>(_stmt);
				if (ifStmt.body.statements.empty())
				{
					if (MovableChecker(*ifStmt.condition).movable())
						return {{}};
					else
						return {{ExpressionStatement{ifStmt.location, FunctionalInstruction{
							ifStmt.location,
							solidity::Instruction::POP,
							{std::move(*ifStmt.condition)}
						}}}};
				}
				if (ifStmt.condition->type() == typeid(Literal))
				{
					auto const& condition = boost::get<Literal>(*ifStmt.condition);
					if (isTrue(condition))
						return {{std::move(ifStmt.body.statements)}};
					else if (isFalse(condition))
						return {{}};
				}
			}
			else if (_stmt.type() == typeid(Switch))
			{
				auto& switchStmt = boost::get<Switch>(_stmt);
				if (switchStmt.cases.size() == 1)
				{
					auto& switchCase = switchStmt.cases.front();
					if (switchCase.value)
						return {{If{
							std::move(switchStmt.location),
							make_shared<Expression>(
								FunctionalInstruction{
									locationOf(*switchStmt.expression),
									solidity::Instruction::EQ,
									{*switchStmt.expression, std::move(*switchCase.value)}
								}
						), std::move(switchCase.body)}}};
					else
						return {{ExpressionStatement{
										locationOf(*switchStmt.expression),
										FunctionalInstruction{
											locationOf(*switchStmt.expression),
											solidity::Instruction::POP,
											{*switchStmt.expression}
										}
						}, std::move(switchCase.body)}};
				}
			}
			else if (_stmt.type() == typeid(ForLoop))
			{
				auto& forLoop = boost::get<ForLoop>(_stmt);
				if (
					forLoop.condition->type() == typeid(Literal) &&
					isFalse(boost::get<Literal>(*forLoop.condition))
				)
					return {{std::move(forLoop.pre.statements)}};
			}
			else
				visit(_stmt);
			return {};
		}
	);
}

bool StructuralSimplifier::isTrue(Literal const& _literal)
{
	static YulString const trueString("true");
	return
		(_literal.kind == LiteralKind::Boolean && _literal.value == trueString) ||
		(_literal.kind == LiteralKind::Number && u256(_literal.value.str()) != u256(0))
	;
}

bool StructuralSimplifier::isFalse(Literal const& _literal)
{
	static YulString const falseString("false");
	return
		(_literal.kind == LiteralKind::Boolean && _literal.value == falseString) ||
		(_literal.kind == LiteralKind::Number && u256(_literal.value.str()) == u256(0))
	;
}
