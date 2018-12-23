type state = {
  countriesRequestStatus: FetchCountries.requestState,
  countries: list(FetchCountries.country),
  selectedCountry: string,
  dropdownVisible: bool,
};

type action =
  | RequestCountries
  | RequestCountriesSuccess(list(FetchCountries.country))
  | RequestCountriesFailure
  | ToggleDropdownVisibility
  | CountrySelected(string);

let component = ReasonReact.reducerComponent("CountrySelect");

let make = (~className, ~country: option(string), ~onChange, _children) => {
  let buttonClick = (event, self) => {
    event->ReactEvent.Mouse.preventDefault;
    self.ReasonReact.send(ToggleDropdownVisibility);
  };

  let countryChange = (country: FetchCountries.country, self) => {
    onChange(country.label);
    self.ReasonReact.send(CountrySelected(country.value));
  };

  {
    ...component,

    didMount: self => self.send(RequestCountries),

    initialState: () => {
      countriesRequestStatus: Init,
      countries: [],
      selectedCountry:
        switch (country) {
        | None => ""
        | Some(text) => text
        },
      dropdownVisible: false,
    },

    reducer: (action, state) =>
      switch (action) {
      | RequestCountries =>
        ReasonReact.UpdateWithSideEffects(
          {...state, countriesRequestStatus: Loading},
          (
            self =>
              Js.Promise.(
                FetchCountries.fetchCountries()
                |> then_(result =>
                     switch (result) {
                     | Some(countries) =>
                       resolve(
                         self.send(RequestCountriesSuccess(countries)),
                       )
                     | None => resolve(self.send(RequestCountriesFailure))
                     }
                   )
                |> ignore
              )
          ),
        )
      | RequestCountriesSuccess(countries) =>
        ReasonReact.Update({
          ...state,
          countries,
          countriesRequestStatus: Success(countries),
        })
      | RequestCountriesFailure =>
        ReasonReact.Update({...state, countriesRequestStatus: Failure})
      | ToggleDropdownVisibility =>
        ReasonReact.Update({
          ...state,
          dropdownVisible: !state.dropdownVisible,
        })
      | CountrySelected(value) =>
        ReasonReact.Update({...state, selectedCountry: value})
      },

    render: self => {
      let {
        countriesRequestStatus,
        countries,
        selectedCountry,
        dropdownVisible,
      } =
        self.state;
      let buttonLabel =
        switch (
          countries
          |> List.find((item: FetchCountries.country) =>
               item.value === selectedCountry
             )
        ) {
        | item => item.label
        | exception Not_found => "Click here!"
        };
      let buttonDisabled =
        switch (countriesRequestStatus) {
        | Success(_countries) => false
        | _ => true
        };

      <div className>
        <Button disabled=buttonDisabled onClick={self.handle(buttonClick)}>
          {ReasonReact.string(buttonLabel)}
        </Button>
        {
          dropdownVisible ?
            <DropdownList
              options=countries
              onChange={self.handle(countryChange)}
            /> :
            ReasonReact.null
        }
      </div>;
    },
  };
};