#include "RuleDatabase.h"

namespace Eliza
{
	RuleDatabase::RuleDatabase()
	{
		CreateDatabase();
	}

	RuleDatabase::~RuleDatabase()
	{
		map<const char*, Rule*>::iterator it = m_RuleDictionary.begin();
		for(; it != m_RuleDictionary.end(); it++)
		{
			delete it->second;
			it->second = NULL;
		}
		delete m_DefaultRule;
	}

	void RuleDatabase::CreateDatabase()
	{
		// Rule1
		{
			Rule* rule = new Rule("hello");
			rule->AddAnswerCandidate("How do you do. Please state your problem.");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule2
		{
			Rule* rule = new Rule("computer");
			rule->AddAnswerCandidate("Do computers worry you?");
			rule->AddAnswerCandidate("What do you think about machine?");
			rule->AddAnswerCandidate("Why do you mention computers?");
			rule->AddAnswerCandidate("What do you think machines have to do with your problem?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule3
		{
			Rule* rule = new Rule("name");
			rule->AddAnswerCandidate("I am not interested in names.");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule4
		{
			Rule* rule = new Rule("sorry");
			rule->AddAnswerCandidate("Please don't apologize");
			rule->AddAnswerCandidate("Apologies are not necessary");
			rule->AddAnswerCandidate("What feelings do you have when you apologizer");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule5
		{
			Rule* rule = new Rule("I remember");
			rule->AddAnswerCandidate("Do you often think of __AfterKey__");
			rule->AddAnswerCandidate("Does thinking of __AfterKey__ bring anything else to mind?");
			rule->AddAnswerCandidate("What else do you remenber");
			rule->AddAnswerCandidate("Why do you recall __AfterKey__ right now?");
			rule->AddAnswerCandidate("What in the present situation reminds you of __AfterKey__");
			rule->AddAnswerCandidate("What is the connection between me and __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule6
		{
			Rule* rule = new Rule("do you remember");
			rule->AddAnswerCandidate("Did you think I would forget __AfterKey__");
			rule->AddAnswerCandidate("Why do you think i should recall __AfterKey__");
			rule->AddAnswerCandidate("What about __AfterKey__");
			rule->AddAnswerCandidate("You mentioned __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule7
		{
			Rule* rule = new Rule("if");
			rule->AddAnswerCandidate("Do you think reallyits likely that __AfterKey__");
			rule->AddAnswerCandidate("Do you with that __AfterKey__");
			rule->AddAnswerCandidate("What do you think about __AfterKey__");
			rule->AddAnswerCandidate("Really-- if __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule8
		{
			Rule* rule = new Rule("I dreamt");
			rule->AddAnswerCandidate("Really-- __AfterKey__");
			rule->AddAnswerCandidate("Have you ever fantasized __AfterKey__ while you were awake");
			rule->AddAnswerCandidate("Have you dreamt __AfterKey__ before");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule9
		{
			Rule* rule = new Rule("dream about");
			rule->AddAnswerCandidate("How do you feel about __AfterKey__ in reallity?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule10
		{
			Rule* rule = new Rule("dream");
			rule->AddAnswerCandidate("What does this dream suggest to you?");
			rule->AddAnswerCandidate("Do you dream often?");
			rule->AddAnswerCandidate("What persons appear in your dreams?");
			rule->AddAnswerCandidate("Don't you believe that dream has to do with your problem?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule11
		{
			Rule* rule = new Rule("my mother");
			rule->AddAnswerCandidate("Who else in your family __AfterKey__");
			rule->AddAnswerCandidate("Tell me more about your family");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule12
		{
			Rule* rule = new Rule("my father");
			rule->AddAnswerCandidate("Your father");
			rule->AddAnswerCandidate("Does he influence you strongly?");
			rule->AddAnswerCandidate("What else comes to mind when you think of your father?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule13
		{
			Rule* rule = new Rule("I want");
			rule->AddAnswerCandidate("What would it mean if you got __AfterKey__");
			rule->AddAnswerCandidate("Why do you want __AfterKey__");
			rule->AddAnswerCandidate("Suppose you got __AfterKey__ soon");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule14
		{
			Rule* rule = new Rule("I am glad");
			rule->AddAnswerCandidate("How have I  helped you to be __AfterKey__");
			rule->AddAnswerCandidate("What mekes you happy just now");
			rule->AddAnswerCandidate("Can you explain why you are suddenly __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule15
		{
			Rule* rule = new Rule("I am sad");
			rule->AddAnswerCandidate("I am sorry to hear you are depressed");
			rule->AddAnswerCandidate("I'm sure its not pleasant to be sad");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule16
		{
			Rule* rule = new Rule("are like");
			rule->AddAnswerCandidate("What resemblance do you see between __BeforeKey__ and __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule17
		{
			Rule* rule = new Rule("is like");
			rule->AddAnswerCandidate("In what way is it that __BeforeKey__ is like __AfterKey__");
			rule->AddAnswerCandidate("What resemblance do you see?");
			rule->AddAnswerCandidate("Could there really be some connection?");
			rule->AddAnswerCandidate("How?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule18
		{
			Rule* rule = new Rule("alike");
			rule->AddAnswerCandidate("What similarities are there?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule19
		{
			Rule* rule = new Rule("same");
			rule->AddAnswerCandidate("What other connections do you see?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule20
		{
			Rule* rule = new Rule("I was");
			rule->AddAnswerCandidate("Were you really?");
			rule->AddAnswerCandidate("Perhaps I already knew you were __AfterKey__");
			rule->AddAnswerCandidate("Why do you tell me you were __AfterKey__ now?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule21
		{
			Rule* rule = new Rule("was I");
			rule->AddAnswerCandidate("What if you were __AfterKey__");
			rule->AddAnswerCandidate("Do you thin you were __AfterKey__");
			rule->AddAnswerCandidate("What would it mean if you were __AfterKey__?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule22
		{
			Rule* rule = new Rule("I am");
			rule->AddAnswerCandidate("In what way are you __AfterKey__");
			rule->AddAnswerCandidate("Do you want to be __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule23
		{
			Rule* rule = new Rule("am I");
			rule->AddAnswerCandidate("Do you believe you are __AfterKey__");
			rule->AddAnswerCandidate("Would you want to be __AfterKey__ ?");
			rule->AddAnswerCandidate("You wish I would tell you you are __AfterKey__");
			rule->AddAnswerCandidate("What would it mean if you were __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule24
		{
			Rule* rule = new Rule(" am ");
			rule->AddAnswerCandidate("Why do you say \"AM\"");
			rule->AddAnswerCandidate("I don't understand that");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule25
		{
			Rule* rule = new Rule("are you");
			rule->AddAnswerCandidate("Why are you interested in whether I am __AfterKey__ or not?");
			rule->AddAnswerCandidate("Would you prefer if I weren't __AfterKey__");
			rule->AddAnswerCandidate("Perhaps I am __AfterKey__ in your fantasies");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule26
		{
			Rule* rule = new Rule("you are");
			rule->AddAnswerCandidate("What makes you think I am __AfterKey__ ?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule27
		{
			Rule* rule = new Rule("because");
			rule->AddAnswerCandidate("Is that the real reason?");
			rule->AddAnswerCandidate("What other reasons might there be?");
			rule->AddAnswerCandidate("Does that reason seem to explain anything else?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule28
		{
			Rule* rule = new Rule("were you");
			rule->AddAnswerCandidate("Perhaps I was __AfterKey__");
			rule->AddAnswerCandidate("What do you think?");
			rule->AddAnswerCandidate("What if I had been __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule29
		{
			Rule* rule = new Rule("I can't");
			rule->AddAnswerCandidate("Maybe you could __AfterKey__ now");
			rule->AddAnswerCandidate("What if you could __AfterKey__ ?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule30
		{
			Rule* rule = new Rule("I feel");
			rule->AddAnswerCandidate("Do you often feel __AfterKey__ ?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule31
		{
			Rule* rule = new Rule("I felt");
			rule->AddAnswerCandidate("What other feelings do you have?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule32
		{
			Rule* rule = new Rule("why don't you");
			rule->AddAnswerCandidate("Should you __AfterKey__ yourself?");
			rule->AddAnswerCandidate("Perhaps I will __AfterKey__ in good time");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule33
		{
			Rule* rule = new Rule("yes");
			rule->AddAnswerCandidate("You seem quite positive");
			rule->AddAnswerCandidate("You are sure");
			rule->AddAnswerCandidate("I understand");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule34
		{
			Rule* rule = new Rule("no");
			rule->AddAnswerCandidate("Why not?");
			rule->AddAnswerCandidate("You are being a bit negative");
			rule->AddAnswerCandidate("Are you saying \"NO\" just to be negative");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule35
		{
			Rule* rule = new Rule("someone");
			rule->AddAnswerCandidate("Can you be more specific?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule36
		{
			Rule* rule = new Rule("everyone");
			rule->AddAnswerCandidate("surely not everyone");
			rule->AddAnswerCandidate("Can you think of anyone in particular?");
			rule->AddAnswerCandidate("Who for example?");
			rule->AddAnswerCandidate("You are thinking of a special person");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule37
		{
			Rule* rule = new Rule("always");
			rule->AddAnswerCandidate("Can you think of a specific example");
			rule->AddAnswerCandidate("When?");
			rule->AddAnswerCandidate("What incident are you thinking of?");
			rule->AddAnswerCandidate("Really-- always");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule38
		{
			Rule* rule = new Rule("what");
			rule->AddAnswerCandidate("What do you ask?");
			rule->AddAnswerCandidate("Does that question interest you?");
			rule->AddAnswerCandidate("What is it you really want to know?");
			rule->AddAnswerCandidate("What do you think?");
			rule->AddAnswerCandidate("What comes to your mind when you ask that?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule39
		{
			Rule* rule = new Rule("perhaps");
			rule->AddAnswerCandidate("You do not seem quite certain");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule40
		{
			Rule* rule = new Rule("are");
			rule->AddAnswerCandidate("Did you think they might not be __AfterKey__");
			rule->AddAnswerCandidate("Possibly they are __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// DefaultRule
		{
			Rule* rule = new Rule("");
			rule->AddAnswerCandidate("Very interesting");
			rule->AddAnswerCandidate("I am not sure I understand you fully");
			rule->AddAnswerCandidate("What does that suggest to you");
			rule->AddAnswerCandidate("Please continue");
			rule->AddAnswerCandidate("Go on");
			rule->AddAnswerCandidate("Do you feel strongly about discussing such things?");
			m_DefaultRule = rule;
		}

	}
}

namespace Eliza
{
	RuleDatabase2::RuleDatabase2()
	{
		CreateDatabase();
	}

	RuleDatabase2::~RuleDatabase2()
	{
		map<const char*, Rule*>::iterator it = m_RuleDictionary.begin();
		for(; it != m_RuleDictionary.end(); it++)
		{
			delete it->second;
			it->second = NULL;
		}
		delete m_DefaultRule;
	}

	void RuleDatabase2::CreateDatabase()
	{
		// Rule1
		{
			Rule* rule = new Rule("こんにちは");
			rule->AddAnswerCandidate("こんにちは!! 現在の問題を教えてください。");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule2
		{
			Rule* rule = new Rule("computer");
			rule->AddAnswerCandidate("Do computers worry you?");
			rule->AddAnswerCandidate("What do you think about machine?");
			rule->AddAnswerCandidate("Why do you mention computers?");
			rule->AddAnswerCandidate("What do you think machines have to do with your problem?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule3
		{
			Rule* rule = new Rule("名前");
			rule->AddAnswerCandidate("私は名前に興味がありません。");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule4
		{
			Rule* rule = new Rule("ごめんなさい");
			rule->AddAnswerCandidate("謝らないで下さい。");
			rule->AddAnswerCandidate("謝罪は必要ありません。");
			rule->AddAnswerCandidate("あなたは謝罪するときどのような気持ちになりますか？");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule5
		{
			Rule* rule = new Rule("I remember");
			rule->AddAnswerCandidate("Do you often think of __AfterKey__");
			rule->AddAnswerCandidate("Does thinking of __AfterKey__ bring anything else to mind?");
			rule->AddAnswerCandidate("What else do you remenber");
			rule->AddAnswerCandidate("Why do you recall __AfterKey__ right now?");
			rule->AddAnswerCandidate("What in the present situation reminds you of __AfterKey__");
			rule->AddAnswerCandidate("What is the connection between me and __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule6
		{
			Rule* rule = new Rule("do you remember");
			rule->AddAnswerCandidate("Did you think I would forget __AfterKey__");
			rule->AddAnswerCandidate("Why do you think i should recall __AfterKey__");
			rule->AddAnswerCandidate("What about __AfterKey__");
			rule->AddAnswerCandidate("You mentioned __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule7
		{
			Rule* rule = new Rule("if");
			rule->AddAnswerCandidate("Do you think reallyits likely that __AfterKey__");
			rule->AddAnswerCandidate("Do you with that __AfterKey__");
			rule->AddAnswerCandidate("What do you think about __AfterKey__");
			rule->AddAnswerCandidate("Really-- if __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule8
		{
			Rule* rule = new Rule("I dreamt");
			rule->AddAnswerCandidate("Really-- __AfterKey__");
			rule->AddAnswerCandidate("Have you ever fantasized __AfterKey__ while you were awake");
			rule->AddAnswerCandidate("Have you dreamt __AfterKey__ before");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule9
		{
			Rule* rule = new Rule("dream about");
			rule->AddAnswerCandidate("How do you feel about __AfterKey__ in reallity?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule10
		{
			Rule* rule = new Rule("dream");
			rule->AddAnswerCandidate("What does this dream suggest to you?");
			rule->AddAnswerCandidate("Do you dream often?");
			rule->AddAnswerCandidate("What persons appear in your dreams?");
			rule->AddAnswerCandidate("Don't you believe that dream has to do with your problem?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule11
		{
			Rule* rule = new Rule("my mother");
			rule->AddAnswerCandidate("Who else in your family __AfterKey__");
			rule->AddAnswerCandidate("Tell me more about your family");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule12
		{
			Rule* rule = new Rule("my father");
			rule->AddAnswerCandidate("Your father");
			rule->AddAnswerCandidate("Does he influence you strongly?");
			rule->AddAnswerCandidate("What else comes to mind when you think of your father?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule13
		{
			Rule* rule = new Rule("I want");
			rule->AddAnswerCandidate("What would it mean if you got __AfterKey__");
			rule->AddAnswerCandidate("Why do you want __AfterKey__");
			rule->AddAnswerCandidate("Suppose you got __AfterKey__ soon");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule14
		{
			Rule* rule = new Rule("I am glad");
			rule->AddAnswerCandidate("How have I  helped you to be __AfterKey__");
			rule->AddAnswerCandidate("What mekes you happy just now");
			rule->AddAnswerCandidate("Can you explain why you are suddenly __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule15
		{
			Rule* rule = new Rule("I am sad");
			rule->AddAnswerCandidate("I am sorry to hear you are depressed");
			rule->AddAnswerCandidate("I'm sure its not pleasant to be sad");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule16
		{
			Rule* rule = new Rule("are like");
			rule->AddAnswerCandidate("What resemblance do you see between __BeforeKey__ and __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule17
		{
			Rule* rule = new Rule("is like");
			rule->AddAnswerCandidate("In what way is it that __BeforeKey__ is like __AfterKey__");
			rule->AddAnswerCandidate("What resemblance do you see?");
			rule->AddAnswerCandidate("Could there really be some connection?");
			rule->AddAnswerCandidate("How?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule18
		{
			Rule* rule = new Rule("alike");
			rule->AddAnswerCandidate("What similarities are there?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule19
		{
			Rule* rule = new Rule("same");
			rule->AddAnswerCandidate("What other connections do you see?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule20
		{
			Rule* rule = new Rule("I was");
			rule->AddAnswerCandidate("Were you really?");
			rule->AddAnswerCandidate("Perhaps I already knew you were __AfterKey__");
			rule->AddAnswerCandidate("Why do you tell me you were __AfterKey__ now?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule21
		{
			Rule* rule = new Rule("was I");
			rule->AddAnswerCandidate("What if you were __AfterKey__");
			rule->AddAnswerCandidate("Do you thin you were __AfterKey__");
			rule->AddAnswerCandidate("What would it mean if you were __AfterKey__?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule22
		{
			Rule* rule = new Rule("I am");
			rule->AddAnswerCandidate("In what way are you __AfterKey__");
			rule->AddAnswerCandidate("Do you want to be __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule23
		{
			Rule* rule = new Rule("am I");
			rule->AddAnswerCandidate("Do you believe you are __AfterKey__");
			rule->AddAnswerCandidate("Would you want to be __AfterKey__ ?");
			rule->AddAnswerCandidate("You wish I would tell you you are __AfterKey__");
			rule->AddAnswerCandidate("What would it mean if you were __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule24
		{
			Rule* rule = new Rule(" am ");
			rule->AddAnswerCandidate("Why do you say \"AM\"");
			rule->AddAnswerCandidate("I don't understand that");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule25
		{
			Rule* rule = new Rule("are you");
			rule->AddAnswerCandidate("Why are you interested in whether I am __AfterKey__ or not?");
			rule->AddAnswerCandidate("Would you prefer if I weren't __AfterKey__");
			rule->AddAnswerCandidate("Perhaps I am __AfterKey__ in your fantasies");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule26
		{
			Rule* rule = new Rule("you are");
			rule->AddAnswerCandidate("What makes you think I am __AfterKey__ ?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule27
		{
			Rule* rule = new Rule("because");
			rule->AddAnswerCandidate("Is that the real reason?");
			rule->AddAnswerCandidate("What other reasons might there be?");
			rule->AddAnswerCandidate("Does that reason seem to explain anything else?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule28
		{
			Rule* rule = new Rule("were you");
			rule->AddAnswerCandidate("Perhaps I was __AfterKey__");
			rule->AddAnswerCandidate("What do you think?");
			rule->AddAnswerCandidate("What if I had been __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule29
		{
			Rule* rule = new Rule("I can't");
			rule->AddAnswerCandidate("Maybe you could __AfterKey__ now");
			rule->AddAnswerCandidate("What if you could __AfterKey__ ?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule30
		{
			Rule* rule = new Rule("I feel");
			rule->AddAnswerCandidate("Do you often feel __AfterKey__ ?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule31
		{
			Rule* rule = new Rule("I felt");
			rule->AddAnswerCandidate("What other feelings do you have?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule32
		{
			Rule* rule = new Rule("why don't you");
			rule->AddAnswerCandidate("Should you __AfterKey__ yourself?");
			rule->AddAnswerCandidate("Perhaps I will __AfterKey__ in good time");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule33
		{
			Rule* rule = new Rule("yes");
			rule->AddAnswerCandidate("You seem quite positive");
			rule->AddAnswerCandidate("You are sure");
			rule->AddAnswerCandidate("I understand");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule34
		{
			Rule* rule = new Rule("no");
			rule->AddAnswerCandidate("Why not?");
			rule->AddAnswerCandidate("You are being a bit negative");
			rule->AddAnswerCandidate("Are you saying \"NO\" just to be negative");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule35
		{
			Rule* rule = new Rule("someone");
			rule->AddAnswerCandidate("Can you be more specific?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule36
		{
			Rule* rule = new Rule("everyone");
			rule->AddAnswerCandidate("surely not everyone");
			rule->AddAnswerCandidate("Can you think of anyone in particular?");
			rule->AddAnswerCandidate("Who for example?");
			rule->AddAnswerCandidate("You are thinking of a special person");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule37
		{
			Rule* rule = new Rule("always");
			rule->AddAnswerCandidate("Can you think of a specific example");
			rule->AddAnswerCandidate("When?");
			rule->AddAnswerCandidate("What incident are you thinking of?");
			rule->AddAnswerCandidate("Really-- always");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule38
		{
			Rule* rule = new Rule("what");
			rule->AddAnswerCandidate("What do you ask?");
			rule->AddAnswerCandidate("Does that question interest you?");
			rule->AddAnswerCandidate("What is it you really want to know?");
			rule->AddAnswerCandidate("What do you think?");
			rule->AddAnswerCandidate("What comes to your mind when you ask that?");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule39
		{
			Rule* rule = new Rule("perhaps");
			rule->AddAnswerCandidate("You do not seem quite certain");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// Rule40
		{
			Rule* rule = new Rule("are");
			rule->AddAnswerCandidate("Did you think they might not be __AfterKey__");
			rule->AddAnswerCandidate("Possibly they are __AfterKey__");
			m_RuleDictionary.insert(pair<const char*, Rule*>(rule->GetRuleKey().c_str(), rule));
		}

		// DefaultRule
		{
			Rule* rule = new Rule("");
			rule->AddAnswerCandidate("Very interesting");
			rule->AddAnswerCandidate("I am not sure I understand you fully");
			rule->AddAnswerCandidate("What does that suggest to you");
			rule->AddAnswerCandidate("Please continue");
			rule->AddAnswerCandidate("Go on");
			rule->AddAnswerCandidate("Do you feel strongly about discussing such things?");
			m_DefaultRule = rule;
		}

	}
}