import React from 'react';
import { View, TouchableOpacity, StyleSheet, FlatList, Text } from 'react-native';
import { useNavigation } from '@react-navigation/native';
import { colors } from "./constants";

const HandleWords = ({ words, handleRemoveWord }) => {
  const navigation = useNavigation();

  const renderItem = ({ item }) => (
    <View style={styles.wordContainer}>
      <Text style={styles.word}>{item}</Text>
      <TouchableOpacity style={styles.button} onPress={() => handleRemoveWord(item)}>
        <Text style={styles.buttonText}>Remover</Text>
      </TouchableOpacity>
    </View>
  );

  return (
    <View style={styles.wrapper}>
      <View style={styles.container}>
        <TouchableOpacity
          style={styles.button}
          onPress={() => navigation.navigate('AddWordScreen')}
        >
          <Text style={styles.buttonText}>Adicionar</Text>
        </TouchableOpacity>
      </View>
      <FlatList
        data={words}
        renderItem={renderItem}
        keyExtractor={item => item}
        contentContainerStyle={styles.list}
      />
    </View>
  );
};

const styles = StyleSheet.create({
  wrapper: {
    flex: 1,
    padding: 20,
    backgroundColor: colors.background,
  },
  container: {
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'center',
    marginBottom: 20,
  },
  list: {
    paddingTop: 20,
  },
  wordContainer: {
    borderWidth: 1,
    borderColor: '#ccc',
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 10,
    backgroundColor: colors.background,
    padding: 10,
    borderRadius: 5,
  },
  word: {
    fontSize: 18,
    color: 'white'
  },
  button: {
    backgroundColor: colors.grey,
    paddingVertical: 10,
    paddingHorizontal: 20,
    borderRadius: 5,
  },
  buttonText: {
    color: 'white',
    fontSize: 16,
    fontWeight: 'bold',
  },
});

export default HandleWords;
