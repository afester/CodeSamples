import React from "react";
import {Button, Form} from "react-bootstrap";
import {loginAction} from "./actions";
import {useDispatch} from "react-redux";

function LoginMask(props) {
    const dispatch = useDispatch();

    return <Form className={'recipeForm'}>
                <Form.Group>
                    <Form.Label>Username:</Form.Label>
                    <Form.Control type="text" placeholder="Username" />
                </Form.Group>

                <Form.Group>
                    <Form.Label>Password:</Form.Label>
                    <Form.Control type="password" placeholder="Password" />
                </Form.Group>

                <Button variant="primary" onClick={() => dispatch(loginAction())}>
                    Login
                </Button>
           </Form>;
}


export default LoginMask;
