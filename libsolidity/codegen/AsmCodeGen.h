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
/**
 * Adaptor between the abstract assembly and eth assembly.
 */

#pragma once

#include <libyul/AsmAnalysis.h>

#include <functional>

namespace yul
{
struct Block;
}

namespace dev
{
namespace eth
{
class Assembly;
}

namespace solidity
{

class CodeGenerator
{
public:
	/// Performs code generation and appends generated to _assembly.
	static void assemble(
		yul::Block const& _parsedData,
		yul::AsmAnalysisInfo& _analysisInfo,
		dev::eth::Assembly& _assembly,
		yul::ExternalIdentifierAccess const& _identifierAccess = yul::ExternalIdentifierAccess(),
		bool _useNamedLabelsForFunctions = false
	);
};

}
}
