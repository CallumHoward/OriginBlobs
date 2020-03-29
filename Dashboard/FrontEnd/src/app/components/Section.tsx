import React, { FunctionComponent } from "react";
import styled from "styled-components";

const StyledDiv = styled.div`
  background-color: #eee;
  border-radius: 3px;
  width: 100vw;
`;

type Props = {
  children: React.ReactNode;
};

const Section: FunctionComponent<Props> = (props) => {
  console.log(props.children);
  return <StyledDiv>{props.children}</StyledDiv>;
};

export default Section;
