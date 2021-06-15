#! /usr/bin/env elixir

target = "lib/Generated/modifier_field_selectors.cpp"

Mix.install([:exprotobuf], consolidate_protocols: false)

defmodule Schemas do
  use Protobuf, from: "command_schemas.proto"
end

template = """
#include <Modifiers.h>

#define MIN_INDEX 3

int *Modifiers::value_from_type_param(CommandParams *cmd, uint8_t field_index)
{
  switch (cmd->which_type_params)
  {
  <%= for {type_name, fields} <- types do %>
  case CommandParams_<%= type_name %>_tag:
    switch (field_index)
    {
    <%= for {field_name, index} <- fields do %>
    case MIN_INDEX + <%= index %>:
      return &cmd->type_params.<%= type_name %>.<%= field_name %>;
    <% end %>
    }
  <% end %>
  }
}
"""

fields_defs = Schemas.CommandParams.defs()
%{fields: fields} = Schemas.CommandParams.defs(:field, :type_params)

types =
  fields
  |> Enum.map(fn %{type: type, name: name} ->
    {_, fields} = Enum.find(fields_defs, &match?({^type, _}, &1))

    fields =
      fields
      |> Enum.filter(&match?(%{type: :int32}, &1))
      |> Enum.map(fn %{name: name} -> name end)
      |> Enum.with_index()

    {name, fields}
  end)

EEx.eval_string(template, types: types)
|> then(&File.write(target, &1))
