type state = {
  filteredCountries: list(FetchCountries.country),
  text: string,
};

type action =
  | InputChange(string);

let component = ReasonReact.reducerComponent("Dropdown");

let make = (~options, ~onChange, _children) => {
  ...component,
  initialState: () => {filteredCountries: options, text: ""},
  reducer: (action, _state) =>
    switch (action) {
    | InputChange(text) =>
      ReasonReact.Update({
        filteredCountries:
          options
          |> List.filter((country: FetchCountries.country) =>
               Js.Re.fromStringWithFlags(text, ~flags="i")
               |> Js.Re.test(country.label)
             ),
        text,
      })
    },
  render: self => {
    let {filteredCountries} = self.state;

    <div>
      <TextInput
        placeholder="Search"
        onChange=(text => self.send(InputChange(text)))
      />
      (
        filteredCountries
        |> List.map((country: FetchCountries.country) =>
             <DropdownListItem key=country.value country onClick=onChange />
           )
        |> Array.of_list
        |> ReasonReact.array
      )
    </div>;
  },
};