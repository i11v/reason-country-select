let getIntFromStr: string => int =
  s =>
    [%bs.re "/^\d+/"]
    |> Js.Re.exec(s)
    |> (
      res =>
        switch (res) {
        | Some(result) =>
          switch (Js.Nullable.toOption(Js.Re.captures(result)[0])) {
          | Some(n) => int_of_string(n)
          | None => 0
          }
        | None => 0
        }
    );

type state = {
  scrollPosition: int,
  visibleHeight: int,
  listWrapperRef: ref(option(Dom.element)),
};

type action =
  | SetVisibleHeight(int)
  | Scroll(int);

module Styles = {
  open Css;

  let container = style([position(`relative)]);

  let listWrapper =
    style([
      position(`absolute),
      top(zero),
      right(zero),
      bottom(zero),
      left(zero),
      overflowY(`auto),
    ]);

  let list = listHeight =>
    style([position(`relative), height(px(listHeight))]);

  let item = (index, itemHeight) =>
    style([
      position(`absolute),
      top(px(index * itemHeight)),
      right(zero),
      left(zero),
    ]);
};

let component = ReasonReact.reducerComponent("VirtualizedList");

let make =
    (
      ~className="",
      ~options,
      ~optionHeight=26,
      ~count=5,
      ~renderItem,
      _children,
    ) => {
  let setListWrapperRef = (theRef, {ReasonReact.state}) =>
    state.listWrapperRef := Js.Nullable.toOption(theRef);

  let setScrollPosition = (event: Dom.event, self) => {
    let getScrollTop: Dom.event => int = [%raw "e => e.target.scrollTop"];
    self.ReasonReact.send(Scroll(getScrollTop(event)));
  };

  let itemIsVisible = (index, {ReasonReact.state}) => {
    let {scrollPosition, visibleHeight} = state;
    let pos = index * optionHeight;
    let offset = count * optionHeight;
    let upperBound = scrollPosition - offset;
    let lowerBound = scrollPosition + visibleHeight + offset;

    pos > upperBound && pos + optionHeight < lowerBound;
  };

  {
    ...component,
    initialState: () => {
      scrollPosition: 0,
      visibleHeight: 0,
      listWrapperRef: ref(None),
    },
    reducer: (action, state) =>
      switch (action) {
      | SetVisibleHeight(visibleHeight) =>
        ReasonReact.Update({...state, visibleHeight})
      | Scroll(scrollPosition) =>
        ReasonReact.Update({...state, scrollPosition})
      },
    didMount: self => {
      let handleScroll = self.handle(setScrollPosition);

      Webapi.Dom.(
        switch (self.state.listWrapperRef^) {
        | None => ()
        | Some(ref_) =>
          Element.addEventListenerUseCapture("scroll", handleScroll, ref_);
          self.onUnmount(() =>
            Element.removeEventListenerUseCapture(
              "scroll",
              handleScroll,
              ref_,
            )
          );
          let listWrapperHeight =
            window
            |> Window.getComputedStyle(ref_)
            |> CssStyleDeclaration.getPropertyValue("height")
            |> getIntFromStr;
          self.send(SetVisibleHeight(listWrapperHeight));
        }
      );
    },
    render: self => {
      let listHeight = List.length(options) * optionHeight;

      <div className=(Css.merge([className, Styles.container]))>
        <div
          className=Styles.listWrapper ref=(self.handle(setListWrapperRef))>
          <div className=(Styles.list(listHeight))>
            (
              options
              |> List.mapi((index, country: FetchCountries.country) =>
                   itemIsVisible(index, self) ?
                     <span
                       key=country.value
                       className=(Styles.item(index, optionHeight))>
                       (renderItem(country))
                     </span> :
                     ReasonReact.null
                 )
              |> Array.of_list
              |> ReasonReact.array
            )
          </div>
        </div>
      </div>;
    },
  };
};