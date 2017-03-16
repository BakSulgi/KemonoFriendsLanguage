#include "Compiler.h"

#include <stdexcept>




Compiler::Compiler()
	: m_tokenList(
{
	&m_stringToken, &m_numberToken,
	&m_funcToken, &m_callToken, &m_mainToken,
	&m_tanoshiToken, &m_sugoiToken,
	&m_uwaToken, &m_waiToken,
	&m_naniToken, &m_omoshiToken,
	&m_lalaToken, &m_myaToken,
	&m_writeRegToken, &m_readRegToken,
}
	)
	
	, m_stringToken(Program::Types::STR, { "\"" })
	, m_numberToken(Program::Types::NUM, { "\'" })
	, m_funcToken({ "friends", "������", "�ի��" })
	, m_callToken(m_funcTable, { "sandstar", "���彺Ÿ", "����ɫ���" })
	, m_mainToken({ "youkoso", "���ڼ�", "�誦����" })
	, m_tanoshiToken(Program::Types::TAN, { "ta", "Ÿ", "��" })
	, m_sugoiToken(Program::Types::SUG, { "sugo", "����", "����", "����", "���ê�" })
	, m_uwaToken({ "u", "��", "��" })
	, m_waiToken({ "wa", "��", "��" })
	, m_naniToken(Program::Types::NANI, { "nanikore", "�����ڷ�", "�ʪ˪���" })
	, m_omoshiToken(Program::Types::OMOS, { "omoshiro", "����÷�", "���⪷��" })
	, m_lalaToken(Program::Types::LAL, { "la", "��", "��", "��" })
	, m_myaToken(Program::Types::MYA, { "mya", "��", "��", "�߫�" })
	, m_writeRegToken(Program::Types::SARU, { "shaberu", "������", "����٪�" })
	, m_readRegToken(Program::Types::SABT, { "shabetta", "shabeta", "����Ÿ", "����Ÿ", "����٪ê�" })
{
	
}

//###################################################################################################

Program Compiler::compile(const std::vector<std::string>& tokens)
{
	// �ʱ�ȭ.
	m_funcTable.clear();


	Program program;


	auto itr = tokens.begin();
	std::size_t index = 0;

	// �Լ� ���̺� �ۼ�.
	for (; itr != tokens.end(); ++itr, ++index)
	{
		if (m_funcToken.check(*itr))
		{
			if (index + 1 >= tokens.size())
				throw std::exception("There is no name for function.");

			auto result = m_funcTable.insert(std::make_pair(tokens[index + 1], index + 2));

			if(result.second == false)
				throw std::exception("Function already exists.");

			// ������ �̸��̹Ƿ� �ǳ� ��.
			++itr;
			++index;
		}
		else if (m_mainToken.check(*itr))
		{
			// ���α׷� ���� ��ġ ����.
			program.setMain(index + 1);
		}
	}


	itr = tokens.begin();
	index = 0;
	std::size_t skipCount = 0;

	// �߰� �ڵ� ����.
	for (; itr != tokens.end(); ++itr, ++index)
	{
		program.appendUwaiList();


		if (skipCount > 0)
		{
			--skipCount;
			continue;
		}


		bool findToken = false;

		for (auto& tok : m_tokenList)
		{
			auto result = tok->checkAndCompile(itr, *itr, program);

			if (result.first)
			{
				skipCount += result.second;

				findToken = true;
				break;
			}
		}

		if (findToken == false)
		{
			program.pushCmd(Program::Types::NOP);
		}
	}


	// �������� ����� �Լ��� ��ȯ�� �� �ֵ��� ��.
	program.pushCmd(Program::Types::FUNC);


	return program;
}

