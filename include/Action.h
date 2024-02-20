#pragma once
#include <string>

class Action {
  const std::string m_name = "NONE";
  const std::string m_type = "NONE";

  public:
  Action();
  Action(const std::string& name, const std::string& type);

  const std::string& name() const;
  const std::string& type() const;
};
