import React, { useState } from 'react';
import { NavigationContainer } from '@react-navigation/native';
import { createMaterialBottomTabNavigator } from '@react-navigation/material-bottom-tabs';
import { createStackNavigator } from '@react-navigation/stack';
import { StatusBar } from 'expo-status-bar';
import { StyleSheet, SafeAreaView, Text } from 'react-native';
import Termo from './src/Termo';
import HandleWords from './src/HandleWords';
import AddWordScreen from './src/AddWordScreen';
import { colors } from "./src/constants";
import { words as initialWords } from './src/words';

const Tab = createMaterialBottomTabNavigator();
const Stack = createStackNavigator();

const TermoScreen = ({ words }) => (
  <SafeAreaView style={styles.container}>
    <Text style={styles.title}>TERMO</Text>
    <Termo words={words} />
  </SafeAreaView>
);

const TermoStack = ({ words }) => (
  <Stack.Navigator screenOptions={{ headerShown: false }}>
    <Stack.Screen name="TermoScreen">
      {props => <TermoScreen {...props} words={words} />}
    </Stack.Screen>
  </Stack.Navigator>
);

const WordsStack = ({ words, handleRemoveWord, handleAddWord }) => (
  <Stack.Navigator>
    <Stack.Screen name="HandleWords" options={{ headerShown: false }}>
      {props => (
        <SafeAreaView style={styles.container}>
          <Text style={styles.title}>TERMO</Text>
          <HandleWords {...props} words={words} handleRemoveWord={handleRemoveWord} />
        </SafeAreaView>
      )}
    </Stack.Screen>
    <Stack.Screen 
      name="AddWordScreen"
      options={{ 
        title: 'Adicionar Palavra',
        headerStyle: { backgroundColor: colors.background },
        headerTintColor: colors.lightgrey,
      }}
    >
      {props => (
        <AddWordScreen {...props} handleAddWord={handleAddWord} />
      )}
    </Stack.Screen>
  </Stack.Navigator>
);

const App = () => {
  const [words, setWords] = useState(initialWords);

  const handleAddWord = (newWord) => {
    const updatedWords = [...words, newWord.toUpperCase()];
    setWords(updatedWords);
  };

  const handleRemoveWord = (wordToRemove) => {
    const updatedWords = words.filter(word => word !== wordToRemove);
    setWords(updatedWords);
  };

  return (
    <SafeAreaView style={styles.container}>
      <StatusBar style="light" />
      <NavigationContainer>
        <Tab.Navigator
          barStyle={{ backgroundColor: colors.grey }}
          activeColor="#FFFFFF"
          inactiveColor="#FFFFFF"
        >
          <Tab.Screen name='Termo'>
            {props => <TermoStack {...props} words={words} />}
          </Tab.Screen>
          <Tab.Screen name='Gerenciar palavras'>
            {props => (
              <WordsStack
                {...props}
                words={words}
                handleRemoveWord={handleRemoveWord}
                handleAddWord={handleAddWord}
              />
            )}
          </Tab.Screen>
        </Tab.Navigator>
      </NavigationContainer>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: colors.background,
  },
  title: {
    color: colors.lightgrey,
    fontSize: 32,
    fontWeight: 'bold',
    marginTop: 25,
    textAlign: 'center',
  }
});

export default App;
