import React from "react";
import { Navbar, Nav, Container } from "react-bootstrap";
import Toggle from "./Toggler";
import Battery from "./Battery";
import SignalStrength from "./SignalStrength";
import Rover from "../assets/rover.svg";
import styled from "styled-components";

const HomeNavBar = (props) => {
  return (
    <React.Fragment>
      <Navbar bg="dark" variant="dark">
        <Container>
          <Navbar.Brand href="/">
            <ImgContainer src={Rover} />
          </Navbar.Brand>
          <Nav className="mr-auto">
            <Nav.Link href="/">Home</Nav.Link>
            <Nav.Link href="/target">Target</Nav.Link>
            <Nav.Link href="/discrete">Discrete</Nav.Link>
          </Nav>
          <Navbar.Collapse className="justify-content-end">
            <Battery />
            <SignalStrength />
            <Toggle theme={props.theme} toggleTheme={props.toggleTheme} />
          </Navbar.Collapse>
        </Container>
      </Navbar>
    </React.Fragment>
  );
};

export default HomeNavBar;

const ImgContainer = styled.img`
  height: 50px;
  width: 50px;
  margin: 1px;
`;
