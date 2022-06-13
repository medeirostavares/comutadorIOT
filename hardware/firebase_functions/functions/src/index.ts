//Link de Acesso:
//https://comutador30amp.web.app/api/v1/comutadores/labin1

import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';
import * as firebaseHelper from 'firebase-functions-helper';
import * as express from 'express';
import * as bodyParser from 'body-parser';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
const app = express(); 
const main = express();

main.use(bodyParser.json());
main.use(bodyParser.urlencoded({extended: false}));
main.use('/api/v1', app);

const comutadoresCollection = 'comutadores';
const usersCollection = 'Users';
//const queryArray = ['comutadorId', '==', 'labin1'];

export const esp32Firestore = functions.https.onRequest(main);

app.patch('/comutadores/:comutadorId', async(req, res) => {
    try {
        await firebaseHelper.firestore
        .updateDocument(db, comutadoresCollection, req.params.comutadorId, req.body);
        res.status(200).send('Update Success!');
    } catch(error) {
        res.status(204).send('Patch Error!');
    }
})

app.patch('/users/:userId', async(req, res) => {
    try {
        await firebaseHelper.firestore
        .updateDocument(db, usersCollection, req.params.userId, req.body);
        res.status(200).send('Update Success!');
    } catch(error) {
        res.status(204).send('Patch Error!');
    }
})

app.get('/comutadores/:comutadorId', async(req, res) => {
    
    try {
        await firebaseHelper.firestore
        .getDocument(db, comutadoresCollection, req.params.comutadorId)
        .then(doc => res.status(200).send(doc));
    } catch(error) {
        res.status(204).send('Get Error!');
    }
})

app.get('/users/:userId', async(req, res) => {
    
    try {
        await firebaseHelper.firestore
        .getDocument(db, usersCollection, req.params.userId)
        .then(doc => res.status(200).send(doc));
    } catch(error) {
        res.status(204).send('Get Error!');
    }
})