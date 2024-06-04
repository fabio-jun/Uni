import React, { useState } from 'react';
import { View, TextInput, Button, StyleSheet, Keyboard, TouchableWithoutFeedback } from 'react-native';
import { colors } from "./constants";

const AddWordScreen = ({ navigation, handleAddWord }) => {
  const [newWord, setNewWord] = useState('');

  const onAddWord = () => {
    if (newWord.trim() !== '') {
      handleAddWord(newWord.trim());
      navigation.goBack();
    }
  };

  return (
    <TouchableWithoutFeedback onPress={Keyboard.dismiss}>
      <View style={styles.container}>
        <TextInput
          style={styles.input}
          value={newWord}
          onChangeText={setNewWord}
          placeholder="Digite uma nova palavra"
          placeholderTextColor={colors.lightgrey}
          autoCapitalize="none"
          autoCorrect={false}
          keyboardType="default"
          returnKeyType="done"
          textContentType="none"
          blurOnSubmit={false}
        />
        <Button title="Adicionar" onPress={onAddWord} color={colors.grey} />
      </View>
    </TouchableWithoutFeedback>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: colors.background,
  },
  input: {
    borderWidth: 1,
    borderColor: '#ccc',
    padding: 10,
    width: '80%',
    marginBottom: 20,
    backgroundColor: 'white',
  },
});

export default AddWordScreen;
